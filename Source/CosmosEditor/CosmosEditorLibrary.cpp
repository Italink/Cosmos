#include "CosmosEditorLibrary.h"
#include "Dom/JsonObject.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "FileHelpers.h"

void UCosmosEditorLibrary::SpawnGS()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	//FString DirectoryPath = TEXT("F:/UnrealProjects/CitySample2/Plugins/GaussianSplattingForUnrealEngine_Private/WorkHome/Cache/Big_City_LVL/CitySample_HLOD0_3DGS");
	FString DirectoryPath = TEXT("D:/ElectricDreamsEnv/Plugins/GaussianSplattingForUnrealEngine_Private/WorkHome/Cache/ElectricDreams_Env/CitySample_HLOD0_3DGS");
	FString ContentPath = TEXT("/Game/3DGS/ElectricDreamsEnv/");
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (World && PlatformFile.DirectoryExists(*DirectoryPath)) {
		IFileManager::Get().IterateDirectory(*DirectoryPath, [World, ContentPath](const TCHAR* Filename, bool bIsDirectory) {
			if (bIsDirectory) {
				FString FolderPart, FileNamePart, ExtensionPart;
				FPaths::Split(Filename, FolderPart, FileNamePart, ExtensionPart);
				FString PlyPath = FString::Printf(TEXT("%s/Cluster0/output/point_cloud/iteration_7000/point_cloud_clipped.ply"), Filename);
				FString JsonPath = FString::Printf(TEXT("%s/Cluster0/output/point_cloud/iteration_7000/point_cloud_info.json"), Filename);

				UPackage* NewPackage = CreatePackage(*(ContentPath + FileNamePart));
				FPackagePath PackagePath = FPackagePath::FromPackageNameChecked(NewPackage->GetName());
				FString PackageLocalPath = PackagePath.GetLocalFullPath();
				//UNiagaraSystem* Niagara = GaussianSplattingUtils::LoadSplatPlyAsParticles(PlyPath, NewPackage, *FileNamePart);

				//UNiagaraSystem* Niagara = LoadObject<UNiagaraSystem>(nullptr, *FString::Printf(TEXT("/Game/3DGS/CitySample/%s.%s"), *FileNamePart, *FileNamePart));
				/*if (Niagara) {
					FString JsonString;
					TSharedPtr<FJsonObject> OutJsonObject;
					if (FFileHelper::LoadFileToString(JsonString, *JsonPath)){
						TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
						if (FJsonSerializer::Deserialize(JsonReader, OutJsonObject) && OutJsonObject.IsValid()){
							TArray<TSharedPtr<FJsonValue>> LocationArray = OutJsonObject->GetArrayField(TEXT("Location"));
							FVector Location;
							Location.X = LocationArray[0]->AsNumber();
							Location.Y = LocationArray[1]->AsNumber();
							Location.Z = LocationArray[2]->AsNumber();

							TArray<TSharedPtr<FJsonValue>> BoxExtentArray = OutJsonObject->GetArrayField(TEXT("BoxExtent"));
							FVector BoxExtent;
							BoxExtent.X = BoxExtentArray[0]->AsNumber();
							BoxExtent.Y = BoxExtentArray[1]->AsNumber();
							BoxExtent.Z = BoxExtentArray[2]->AsNumber();

							ANiagaraActor* Actor = World->SpawnActor<ANiagaraActor>(Location, FRotator());
							if (Actor) {
								Actor->GetNiagaraComponent()->SetAsset(Niagara);
								Actor->Modify();
								Actor->SetActorLabel(FileNamePart);
							}
							bool bPromptUserToSave = false;
							bool bSaveMapPackages = true;
							bool bSaveContentPackages = true;
							FEditorFileUtils::SaveDirtyPackages(bPromptUserToSave, bSaveMapPackages, bSaveContentPackages);
						}
					}
				}*/
			}
			return true;
		});
	}
}

