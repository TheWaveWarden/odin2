#include "LiveConstrainer.h"
#include "../ConfigFileManager.h"
#include "../PluginEditor.h"

LiveConstrainer::LiveConstrainer(OdinEditor &p_editor) : m_main_editor(p_editor) {
	juce::Desktop::getInstance().addGlobalMouseListener(this);
}

LiveConstrainer::~LiveConstrainer() {
	juce::Desktop::getInstance().removeGlobalMouseListener(this);
}

void LiveConstrainer::mouseDown(const juce::MouseEvent &p_event) {
	if (p_event.eventComponent != m_component) {
		unregisterCurrentComponent();
		if (p_event.mods.isShiftDown() && p_event.mods.isCtrlDown() && p_event.mods.isAltDown()) {

			if (p_event.mods.isMiddleButtonDown()) {
				p_event.eventComponent->setVisible(false);
				return;
			}

			m_component = p_event.eventComponent;
			jassert(m_component);
			m_was_enabled = m_component->isEnabled();

			// todo add again
			//m_component->setEnabled (false);

			if (m_component->getProperties().contains(GUI_DATA_NAME)) {
				m_name          = m_component->getProperties()[GUI_DATA_NAME].toString().toStdString();
				m_bounds_scaled = JsonGuiProvider::getInstance().getBounds(m_name);
				m_bounds        = JsonGuiProvider::getInstance().getBoundsUnscaled(m_name);
				m_new_bounds    = m_bounds;
				m_drag_start    = p_event.position;

				m_component->setInterceptsMouseClicks(true, false);

				DBG("Constraining component: " << m_name);

				m_main_editor.repaint();
			}
		}
	} else {
		m_drag_mode = cursorToDragMode(positionToMouseCursor(p_event.position));
	}

	MouseListener::mouseDown(p_event);
}

void LiveConstrainer::mouseMove(const juce::MouseEvent &p_event) {
	if (m_component != p_event.eventComponent) {
		return;
	}

	if (m_drag_mode == DragMode::None) {
		if (m_component == p_event.eventComponent) {
			m_component->setMouseCursor(positionToMouseCursor(p_event.position));
		} else {
		}
	} else {
	}

	MouseListener::mouseMove(p_event);
}

void LiveConstrainer::mouseUp(const juce::MouseEvent &p_event) {
	if (m_drag_mode != DragMode::None) {
		m_drag_mode = DragMode::None;

		m_bounds_scaled = JsonGuiProvider::getInstance().getBounds(m_name);
		m_bounds        = JsonGuiProvider::getInstance().getBoundsUnscaled(m_name);
		m_new_bounds    = m_bounds;
	}
	MouseListener::mouseExit(p_event);
}

void LiveConstrainer::mouseDrag(const juce::MouseEvent &p_event) {
	if (m_component != p_event.eventComponent) {
		return;
	}

	const auto scale = int(ConfigFileManager::getInstance().getOptionGuiScale()) * 2.0f;

	switch (m_drag_mode) {
	default:
		break;
	case DragMode::TopLeft:
		break;
	case DragMode::TopRight:
		break;
	case DragMode::BottomRight:
		break;
	case DragMode::BottomLeft:
		break;
	case DragMode::Left:
		if (p_event.getDistanceFromDragStartX() != 0)
			m_new_bounds = m_bounds.withLeft(m_bounds.getX() + p_event.getDistanceFromDragStartX() / scale);
		break;
	case DragMode::Top:
		if (p_event.getDistanceFromDragStartY() != 0)
			m_new_bounds = m_bounds.withTop(m_bounds.getY() + p_event.getDistanceFromDragStartY() / scale);
		break;
	case DragMode::Right:
		if (p_event.getDistanceFromDragStartX() != 0)
			m_new_bounds = m_bounds.withRight(m_bounds.getRight() + p_event.getDistanceFromDragStartX() / scale);
		break;
	case DragMode::Bottom:
		if (p_event.getDistanceFromDragStartY() != 0)
			m_new_bounds = m_bounds.withBottom(m_bounds.getBottom() + p_event.getDistanceFromDragStartY() / scale);
		break;
	case DragMode::Center:
		if (p_event.getDistanceFromDragStartX() != 0 || p_event.getDistanceFromDragStartY() != 0)
			m_new_bounds = m_bounds.withPosition(m_bounds.getPosition() + p_event.getOffsetFromDragStart() / scale);
		break;
		break;
	}
	updateComponent();

	MouseListener::mouseDrag(p_event);
}

void LiveConstrainer::updateComponent() {
	if (m_component && m_component->getParentComponent()) {
		JsonGuiProvider::getInstance().setBoundsUnscaled(m_name, m_new_bounds);
		m_component->getParentComponent()->resized();
		m_main_editor.repaint();
	}
}

void LiveConstrainer::paintOverlay(juce::Graphics &g) {

	if (m_component) {
		constexpr auto stroke_size = 1.0f;

		auto *editor = m_component->findParentComponentOfClass<OdinEditor>();

		// highlgiht component outline
		//auto textBounds = editor->getLocalBounds().withSizeKeepingCentre (editor->proportionOfWidth (0.2f), editor->proportionOfHeight (0.2f));
		auto textBounds = editor->getLocalBounds()
		                      .removeFromRight(editor->proportionOfWidth(0.2f))
		                      .removeFromTop(editor->proportionOfHeight(0.2f));
		g.setColour(juce::Colour(0xff101010));
		g.fillRect(textBounds);
		g.setColour(juce::Colours::grey);
		g.drawRect(textBounds, 1);

		// big info text field
		textBounds.reduce(textBounds.proportionOfHeight(0.1f), textBounds.proportionOfHeight(0.1f));
		const auto line_height = textBounds.proportionOfHeight(1.0f / 6.0f);
		g.setFont(Helpers::getAldrichFont(line_height * 0.8f));
		const auto just = juce::Justification::centredLeft;

		auto new_bounds_scaled = JsonGuiProvider::getInstance().getBounds(m_name);
		auto zoom_size         = ConfigFileManager::getInstance().getOptionGuiScale();

		auto bounds = textBounds.removeFromTop(line_height);
		g.drawText(m_name, bounds, juce::Justification::centredLeft);
		bounds = textBounds.removeFromTop(line_height);
		g.drawText("Zoom: " + juce::String(juce::roundToInt(100.0f / float(GuiScale::Z100) * float(zoom_size))) + "% (x" +
		               juce::String(zoom_size) + ")",
		           bounds,
		           juce::Justification::centredRight);
		bounds = textBounds.removeFromTop(line_height);
		g.drawText("X: " + juce::String(m_new_bounds.getX()), bounds, juce::Justification::centredLeft);
		g.drawText(juce::String(new_bounds_scaled.getX()), bounds, juce::Justification::centredRight);
		bounds = textBounds.removeFromTop(line_height);
		g.drawText("Y: " + juce::String(m_new_bounds.getY()), bounds, juce::Justification::centredLeft);
		g.drawText(juce::String(new_bounds_scaled.getY()), bounds, juce::Justification::centredRight);
		bounds = textBounds.removeFromTop(line_height);
		g.drawText("W: " + juce::String(m_new_bounds.getWidth()), bounds, juce::Justification::centredLeft);
		g.drawText(juce::String(new_bounds_scaled.getWidth()), bounds, juce::Justification::centredRight);
		bounds = textBounds.removeFromTop(line_height);
		g.drawText("H: " + juce::String(m_new_bounds.getHeight()), bounds, juce::Justification::centredLeft);
		g.drawText(juce::String(new_bounds_scaled.getHeight()), bounds, juce::Justification::centredRight);

		auto boundsInEditor = editor->getLocalArea(m_component, m_component->getLocalBounds());
		g.setColour(juce::Colours::red);
		const auto outline_bounds = boundsInEditor.toFloat().reduced(stroke_size / 2);
		g.drawRoundedRectangle(outline_bounds, 2.0f, stroke_size);
		g.setColour(juce::Colours::red.withAlpha(0.35f));
		g.drawLine(outline_bounds.getX(),
		           outline_bounds.getCentreY(),
		           outline_bounds.getRight(),
		           outline_bounds.getCentreY(),
		           stroke_size);
		g.drawLine(outline_bounds.getCentreX(),
		           outline_bounds.getY(),
		           outline_bounds.getCentreX(),
		           outline_bounds.getBottom(),
		           stroke_size);
	}
}

void LiveConstrainer::unregisterCurrentComponent() {
	if (m_component) {
		m_component->setMouseCursor(juce::MouseCursor::NormalCursor);
		m_component->setEnabled(m_was_enabled);
		m_component = nullptr;

		m_main_editor.repaint();
	}
}

juce::MouseCursor LiveConstrainer::positionToMouseCursor(juce::Point<float> p_pos) {
	// if (p_pos.getDistanceFrom(m_bounds_scaled.withPosition(0.0f, 0.0f).toFloat().getTopLeft()) < SNAP_DISTANCE) {
	// 	return juce::MouseCursor::TopLeftCornerResizeCursor;
	// } else if (p_pos.getDistanceFrom(m_bounds_scaled.withPosition(0.0f, 0.0f).toFloat().getTopRight()) <
	//            SNAP_DISTANCE) {
	// 	return juce::MouseCursor::TopRightCornerResizeCursor;
	// } else if (p_pos.getDistanceFrom(m_bounds_scaled.withPosition(0.0f, 0.0f).toFloat().getBottomRight()) <
	//            SNAP_DISTANCE) {
	// 	return juce::MouseCursor::BottomRightCornerResizeCursor;
	// } else if (p_pos.getDistanceFrom(m_bounds_scaled.withPosition(0.0f, 0.0f).toFloat().getBottomLeft()) <
	//            SNAP_DISTANCE) {
	// 	return juce::MouseCursor::BottomLeftCornerResizeCursor;

	const auto scale = int(ConfigFileManager::getInstance().getOptionGuiScale());

	const int snap_distance_x = std::min(m_bounds.getWidth() * scale / 2 - 3, SNAP_DISTANCE);
	const int snap_distance_y = std::min(m_bounds.getHeight() * scale / 2 - 3, SNAP_DISTANCE);

	if (std::abs(p_pos.getX() - 0) < snap_distance_x) {
		return juce::MouseCursor::LeftEdgeResizeCursor;
	} else if (std::abs(p_pos.getY() - 0) < snap_distance_y) {
		return juce::MouseCursor::TopEdgeResizeCursor;
	} else if (std::abs(p_pos.getX() - m_bounds_scaled.getWidth()) < snap_distance_x) {
		return juce::MouseCursor::RightEdgeResizeCursor;
	} else if (std::abs(p_pos.getY() - m_bounds_scaled.getHeight()) < snap_distance_y) {
		return juce::MouseCursor::BottomEdgeResizeCursor;
	}

	return juce::MouseCursor::UpDownLeftRightResizeCursor;
}

LiveConstrainer::DragMode LiveConstrainer::cursorToDragMode(const juce::MouseCursor &p_cursor) {
	if (p_cursor == juce::MouseCursor::TopLeftCornerResizeCursor) {
		return DragMode::TopLeft;
	} else if (p_cursor == juce::MouseCursor::TopRightCornerResizeCursor) {
		return DragMode::TopRight;
	} else if (p_cursor == juce::MouseCursor::BottomRightCornerResizeCursor) {
		return DragMode::BottomRight;
	} else if (p_cursor == juce::MouseCursor::BottomLeftCornerResizeCursor) {
		return DragMode::BottomLeft;
	} else if (p_cursor == juce::MouseCursor::LeftEdgeResizeCursor) {
		return DragMode::Left;
	} else if (p_cursor == juce::MouseCursor::TopEdgeResizeCursor) {
		return DragMode::Top;
	} else if (p_cursor == juce::MouseCursor::RightEdgeResizeCursor) {
		return DragMode::Right;
	} else if (p_cursor == juce::MouseCursor::BottomEdgeResizeCursor) {
		return DragMode::Bottom;
	} else if (p_cursor == juce::MouseCursor::UpDownLeftRightResizeCursor) {
		return DragMode::Center;
	}

	return DragMode::None;
}

bool LiveConstrainer::isConstraining() {
	return bool(m_component);
}