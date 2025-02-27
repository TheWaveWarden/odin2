#pragma once

#include "../GlobalIncludes.h"
#include "RescaleProgressComponent.h"
#include "UIAssetsSize.h"
#include "UIAssetsData.h"

#define UI_ASSET_PATH (ODIN_STORAGE_PATH + File::getSeparatorString() + "UIAssets")

static constexpr auto NUM_UI_SCALE_WORKER_THREADS = 16;

class OdinEditor;

class UIRescalerWorkerThread : public juce::Thread {
public:
	UIRescalerWorkerThread (std::atomic<float>& p_progress, int p_thread_index);

	void run() override;
	void setZoom (unsigned int p_zoom);
	float getProgress() const {
		return m_progress;
	}

private:
	unsigned int m_zoom;
	const int m_thread_index;
	std::atomic<float>& m_progress;
};

class UIRescalerThread : public juce::Thread {
public:
	UIRescalerThread (std::atomic<bool>& p_is_rescaling);

	void run() override;
	void setZoom (unsigned int p_zoom);

private:
	std::array<std::atomic<float>, NUM_UI_SCALE_WORKER_THREADS> m_individual_progress;
	std::array<std::unique_ptr<UIRescalerWorkerThread>, NUM_UI_SCALE_WORKER_THREADS> m_worker_threads;

	std::atomic<bool>& m_is_rescaling;

	unsigned int m_zoom;
};

class UIAssetManager : public juce::DeletedAtShutdown {

public:
	~UIAssetManager() override;

	UIAssetManager (UIAssetManager const&) = delete;
	void operator= (UIAssetManager const&) = delete;

	juce::Image getUIAsset (UIAssets::Indices p_index, unsigned int p_zoom);
	bool launchImageCreationThreads (unsigned int p_zoom);
	void createScaledImageAssets (unsigned int p_zoom, int p_thread_index, std::atomic<float>& p_progress);

	bool isCurrentlyRescaling() const;

	void registerEditor (OdinEditor* p_editor);
	void unregisterEditor (OdinEditor* p_editor);

	void setAllEditorOverlaysVisible (bool p_visible);
	void setAllEditorZoomSize (GuiScale p_size);
	void setAllRescaleProgress (float p_progress);

	JUCE_DECLARE_SINGLETON (UIAssetManager, false)

private:
	UIAssetManager();

	bool assetFolderExistsAndContainsImages() const;
	void popuplateStorageFromFolder();

	int m_zoom_size;
	const juce::File m_asset_folder;
	juce::File m_scaled_asset_folder;

	std::atomic<bool> m_is_rescaling{false};
	std::array<juce::Image, int (UIAssets::Indices::NumIndices)> m_image_storage;

	UIRescalerThread m_rescaler_thread;

	juce::SpinLock m_register_editor_lock;
	std::vector<OdinEditor*> m_registered_editors;
};