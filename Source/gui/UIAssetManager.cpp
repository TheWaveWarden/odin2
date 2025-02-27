#include "UIAssetManager.h"
#include "../ConfigFileManager.h"
#include "../PluginEditor.h"

UIRescalerWorkerThread::UIRescalerWorkerThread(std::atomic<float> &p_progress, int p_thread_index) :
    juce::Thread("UIRescalerWorkerThread"),
    m_progress(p_progress),
    m_thread_index(p_thread_index) {
}

void UIRescalerWorkerThread::run() {
	m_progress.store(0.0f);
	UIAssetManager::getInstance()->createScaledImageAssets(m_zoom, m_thread_index, m_progress);
}

void UIRescalerWorkerThread::setZoom(unsigned int p_zoom) {
	m_zoom = p_zoom;
}

//==============================================================================
//==============================================================================
//==============================================================================

UIRescalerThread::UIRescalerThread(std::atomic<bool> &p_is_rescaling) : juce::Thread("UIRescalerThread"), m_is_rescaling(p_is_rescaling) {
	for (int thread = 0; thread < NUM_UI_SCALE_WORKER_THREADS; ++thread)
		m_worker_threads[thread] = std::make_unique<UIRescalerWorkerThread>(m_individual_progress[thread], thread);
}

void UIRescalerThread::run() {
	UIAssetManager::getInstance()->setAllEditorOverlaysVisible(true);

	const auto time_start = juce::Time::getMillisecondCounter();
	DBG("Start scaling images to size " << int(m_zoom));

	for (auto &worker : m_worker_threads)
		worker->startThread();

	bool all_threads_finished = false;
	while (!all_threads_finished) {
		all_threads_finished = true;
		float progress       = 1.0f;

		for (auto &worker : m_worker_threads) {
			if (worker->isThreadRunning())
				all_threads_finished = false;

			progress = juce::jmin(progress, worker->getProgress());
		}

		UIAssetManager::getInstance()->setAllRescaleProgress(progress);

		if (!all_threads_finished)
			juce::Thread::sleep(50);
	}

	const auto time_end = juce::Time::getMillisecondCounter();
	DBG("Rescaled and wrote images to size " << int(m_zoom) << " in " << (float(time_end - time_start) / 1000.0f) << "s");

	m_is_rescaling.store(false);

	UIAssetManager::getInstance()->setAllEditorOverlaysVisible(false);
	UIAssetManager::getInstance()->setAllEditorZoomSize(GuiScale(m_zoom));
}

void UIRescalerThread::setZoom(unsigned int p_zoom) {
	m_zoom = p_zoom;

	for (auto &worker : m_worker_threads)
		worker->setZoom(p_zoom);
}

//==============================================================================
//==============================================================================
//==============================================================================

JUCE_IMPLEMENT_SINGLETON(UIAssetManager)

UIAssetManager::UIAssetManager() : m_asset_folder(UI_ASSET_PATH), m_rescaler_thread(m_is_rescaling) {
	if (!m_asset_folder.isDirectory())
		m_asset_folder.createDirectory();
}

UIAssetManager::~UIAssetManager() {
	clearSingletonInstance();
}

juce::Image UIAssetManager::getUIAsset(UIAssets::Indices p_index, unsigned int p_zoom) {
	if (m_is_rescaling)
		return juce::Image();

	return m_image_storage[int(p_index)];
}

bool UIAssetManager::isCurrentlyRescaling() const {
	return m_is_rescaling.load();
}

void UIAssetManager::popuplateStorageFromFolder() {
	m_is_rescaling.store(true);

	const auto time_start = juce::Time::getMillisecondCounter();

	for (int i = 0; i < int(UIAssets::Indices::NumIndices); ++i)
		m_image_storage[i] = juce::ImageFileFormat::loadFrom(m_scaled_asset_folder.getChildFile(juce::String(i) + ".png"));

	const auto time_end = juce::Time::getMillisecondCounter();

	DBG("Loaded images from disk in " << (float(time_end - time_start) / 1000.0f) << "s");
	(void)time_start;
	(void)time_end;

	m_is_rescaling.store(false);
}

bool UIAssetManager::launchImageCreationThreads(unsigned int p_zoom) {
	m_zoom_size           = p_zoom;
	m_scaled_asset_folder = m_asset_folder.getChildFile(juce::String(p_zoom));

	if (assetFolderExistsAndContainsImages()) {
		popuplateStorageFromFolder();
		return false;
	}

	if (m_is_rescaling)
		return false;

	if (!m_scaled_asset_folder.isDirectory())
		m_scaled_asset_folder.createDirectory();

	m_rescaler_thread.setZoom(p_zoom);
	m_is_rescaling.store(true);
	m_rescaler_thread.startThread();

	return true;
}

void UIAssetManager::createScaledImageAssets(unsigned int p_zoom, int p_thread_index, std::atomic<float> &p_progress) {

	juce::File target_folder = m_asset_folder.getChildFile(juce::String(int(p_zoom)));
	if (!target_folder.isDirectory())
		target_folder.createDirectory();

	for (int i = p_thread_index; i < int(UIAssets::Indices::NumIndices); i += NUM_UI_SCALE_WORKER_THREADS) {
		const auto progress = float(i) / float(UIAssets::Indices::NumIndices);
		p_progress.store(progress);

		auto unscaled_image = juce::ImageCache::getFromMemory(UIAssetsData::Data[i], UIAssetsSize::Size[i]);
		jassert((unscaled_image.getWidth() % int(GuiScale::Z200)) == 0);
		jassert((unscaled_image.getHeight() % int(GuiScale::Z200)) == 0);

		const auto new_width  = (unscaled_image.getWidth() / int(GuiScale::Z200)) * p_zoom;
		const auto new_height = (unscaled_image.getHeight() / int(GuiScale::Z200)) * p_zoom;

		const auto format = unscaled_image.getFormat();

		juce::Image scaled_image(format, new_width, new_height, true);

		scaled_image = unscaled_image.rescaled(new_width, new_height, juce::Graphics::highResamplingQuality);

		m_image_storage[i] = scaled_image;

		juce::File out_file = target_folder.getChildFile(juce::String(i) + ".png");

		if (out_file.existsAsFile())
			out_file.deleteFile();

		FileOutputStream stream(out_file);
		PNGImageFormat pngWriter;
		pngWriter.writeImageToStream(scaled_image, stream);
	}
}

bool UIAssetManager::assetFolderExistsAndContainsImages() const {
	return m_scaled_asset_folder.isDirectory() && m_scaled_asset_folder.getChildFile("0.png").existsAsFile() &&
	       m_scaled_asset_folder.getChildFile(juce::String(int(UIAssets::Indices::NumIndices) - 1) + ".png").existsAsFile();
}

void UIAssetManager::registerEditor(OdinEditor *p_editor) {
	juce::SpinLock::ScopedLockType lock(m_register_editor_lock);

	if (std::find(m_registered_editors.begin(), m_registered_editors.end(), p_editor) == m_registered_editors.end())
		m_registered_editors.push_back(p_editor);
}

void UIAssetManager::unregisterEditor(OdinEditor *p_editor) {
	juce::SpinLock::ScopedLockType lock(m_register_editor_lock);

	m_registered_editors.erase(std::remove(m_registered_editors.begin(), m_registered_editors.end(), p_editor), m_registered_editors.end());
}

void UIAssetManager::setAllEditorOverlaysVisible(bool p_visible) {
	juce::SpinLock::ScopedLockType lock(m_register_editor_lock);

	for (auto editor : m_registered_editors) {
		juce::WeakReference<OdinEditor> weak_ref = editor;
		juce::MessageManager::callAsync([weak_ref, p_visible]() {
			if (weak_ref.get())
				weak_ref->getRescaleOverlayComponent().setVisible(p_visible);
		});
	}
}

void UIAssetManager::setAllEditorZoomSize(GuiScale p_size) {
	juce::SpinLock::ScopedLockType lock(m_register_editor_lock);

	for (auto editor : m_registered_editors) {
		juce::WeakReference<OdinEditor> weak_ref = editor;
		juce::MessageManager::callAsync([weak_ref, p_size]() {
			if (weak_ref.get())
				weak_ref->setGuiSize(p_size, true);
		});
	}
}

void UIAssetManager::setAllRescaleProgress(float p_progress) {
	juce::SpinLock::ScopedLockType lock(m_register_editor_lock);

	for (auto editor : m_registered_editors) {
		juce::WeakReference<OdinEditor> weak_ref = editor;
		juce::MessageManager::callAsync([weak_ref, p_progress]() {
			if (weak_ref.get())
				weak_ref->getRescaleOverlayComponent().setProgress(p_progress);
		});
	}
}