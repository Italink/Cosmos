#include "GameSettingsUIP.h"

#define LOCTEXT_NAMESPACE "Cosmos"

void UGameSettingsUIP::OnCreateUI()
{
	{
		BeginCategory(LOCTEXT("Gameplay", "Gameplay"));
		EndCategory();
	}

	{
		BeginCategory(LOCTEXT("Input", "Input"));
		EndCategory();
	}

	{
		BeginCategory(LOCTEXT("Interface", "Interface"));
		EndCategory();
	}

	{
		BeginCategory(LOCTEXT("Graphics", "Graphics"));
		{
			BeginGroup(LOCTEXT("Display", "Display"), ESettingGroupType::VBox);

			AddComboBoxItem(LOCTEXT("Monitor", "Monitor"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});

			AddComboBoxItem(LOCTEXT("DisplayAdapter", "Display Adapter"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});

			AddComboBoxItem(LOCTEXT("DisplayMode", "Display Mode"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});

			AddComboBoxItem(LOCTEXT("Resolution", "Resolution"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});

			AddComboBoxItem(LOCTEXT("ScreenRefreshRate", "Screen Refresh Rate"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});

			AddComboBoxItem(LOCTEXT("AspectRatio", "AspectRatio"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});

			AddButtonItem(LOCTEXT("Brightness", "Brightness"), [this]() {
				UE_LOG(LogCmsUI, Warning, TEXT("UI %s Hello"), *GetClass()->GetName());
			});

			AddComboBoxItem(LOCTEXT("FrameRateLimit", "Frame Rate Limit"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});

			AddNumberBoxItem(LOCTEXT("Sharpening", "Sharpening"), 0, 0, 100, 1, [this](float Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Number Changed : %f"), Value);
			});

			AddCheckBoxItem(LOCTEXT("VSync", "VSync"), true, [this](bool Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Check State Changed : %d"), Value);
			});

			EndGroup();
		}

		{
			BeginGroup(LOCTEXT("Camera", "Camera"), ESettingGroupType::VBox);
			AddNumberBoxItem(LOCTEXT("FOV", "FOV"), 0, 0, 100, 1, [this](float Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Number Changed : %f"), Value);
			});
			EndGroup();
		}


		{
			BeginGroup(LOCTEXT("Quality", "Quality"), ESettingGroupType::VBox);
			AddComboBoxItem(LOCTEXT("Preset", "Preset"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("Style", "Style"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("AntiAliasing", "Anti-aliasing Method"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("ReflectionQuality", "Reflection Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("TextureFilteringQuality", "Texture Filtering Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});			
			AddComboBoxItem(LOCTEXT("AmbientOcclusionQuality", "Ambient Occlusion Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("ParticleQuality", "Particle Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("SceneDetailsQuality", "Scene Details Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("SceneViewDistance", "Scene View Distance"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("GlobalIlluminationQuality", "Global Illumination Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("ShaderQuality", "Shader Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("TextureQuality", "Texture Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("StreamingQuality", "Streaming Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("ShadowQuality", "Shadow Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("ShadowMapResolution", "Shadow Map Resolution"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("PostProcessingQuality", "Post Processing Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("VolumetricFogQuality", "Volumetric Fog Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("AnimationQuality", "Animation Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			EndGroup();
		}
		{
			BeginGroup(LOCTEXT("SuperResolution", "Super Resolution"), ESettingGroupType::VBox);
			AddComboBoxItem(LOCTEXT("SuperResolutionMethod", "Super Resolution Method"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("SuperResolutionQuality", "Super Resolution Quality"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("DLSSFrameGeneration", "DLSS Frame Generation"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			AddComboBoxItem(LOCTEXT("NVIDIAREFLEXLowLatency", "NVIDIA REFLEX Low Latency"), { "A","B", "C" }, 0, [this](int Value) {
				UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
			});
			EndGroup();
		}

		EndCategory();
	}

	{
		BeginCategory(LOCTEXT("Audio", "Audio"));
		EndCategory();
	}


	{
		BeginCategory(LOCTEXT("About", "About"));
		EndCategory();
	}


	BeginCategory(LOCTEXT("Category", "Category"));

	BeginGroup(LOCTEXT("Group", "Group"), ESettingGroupType::VBox);

	AddButtonItem(LOCTEXT("Button", "Button"), [this]() {
		UE_LOG(LogCmsUI, Warning, TEXT("UI %s Hello"), *GetClass()->GetName());
	});

	AddNumberBoxItem(LOCTEXT("NumberBox", "NumberBox"), 0, 0 ,100, 1, [this](float Value) {
		UE_LOG(LogCmsUI, Warning, TEXT("Number Changed : %f"), Value);
	});

	AddCheckBoxItem(LOCTEXT("CheckBox", "CheckBox"), true, [this](bool Value) {
		UE_LOG(LogCmsUI, Warning, TEXT("Check State Changed : %d"), Value);
	});

	AddComboBoxItem(LOCTEXT("ComboBox", "ComboBox"), {"A","B", "C"}, 0, [this](int Value) {
		UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %d"), Value);
	});

	AddTextBoxItem(LOCTEXT("TextBox", "TextBox"), LOCTEXT("TextBox", "TextBox"), LOCTEXT("TextBox", "TextBox"),[this](FText Text) {
		UE_LOG(LogCmsUI, Warning, TEXT("Combo Box Index Changed : %s"), *(Text.ToString()));
	});

	EndGroup();

	EndCategory();

	Super::OnCreateUI();
}

#undef LOCTEXT_NAMESPACE
