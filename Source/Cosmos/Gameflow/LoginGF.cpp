#include "LoginGF.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Misc/DynamicLambda/DynamicLambda.h"
#include "Kismet/GameplayStatics.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

ULoginFlow* ULoginFlow::Get(UObject* InWorldContext /*= nullptr*/)
{
	static TObjectPtr<UGameInstance> LastUseGameInstance;
	UGameInstance* GameInstacne = nullptr;
	if (InWorldContext) {
		GameInstacne = UGameplayStatics::GetGameInstance(InWorldContext);
		LastUseGameInstance = GameInstacne;
	}
	else {
		GameInstacne = LastUseGameInstance;
	}
	return GameInstacne->GetSubsystem<ULoginFlow>();
}

void ULoginFlow::Setup()
{
	WorldReady.AddLambda([this]() {
		LoadObjectAsync<ULevelSequence>(TEXT("/Game/Cosmos/Sequence/LS_Startup.LS_Startup"), [this](ULevelSequence* Sequence){
			UWorld* World = GetWorld();
			ACameraActor* CameraActor = World->SpawnActor<ACameraActor>();
			CameraActor->GetCameraComponent()->bConstrainAspectRatio = false;
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
			PlayerController->SetViewTarget(CameraActor);
			FMovieSceneSequencePlaybackSettings Settings;
			ALevelSequenceActor* SequenceActor = nullptr;
			ULevelSequencePlayer::CreateLevelSequencePlayer(this, Sequence, Settings, SequenceActor);
			SequenceActor->SequencePlayer->OnFinished.AddDynamicLambda(nullptr, [this]() {
				LoginSequencePlayFinisehd.Broadcast();
			});
			UMovieScene* MovieScene = Sequence->GetMovieScene();
			check(MovieScene);
			const FMovieSceneBinding* Binding = Algo::FindBy(MovieScene->GetBindings(), FString("CameraActor"), &FMovieSceneBinding::GetName);
			check(Binding);
			EMovieSceneObjectBindingSpace Space = EMovieSceneObjectBindingSpace::Root;
			FMovieSceneObjectBindingID BindingID = UE::MovieScene::FRelativeObjectBindingID(Binding->GetObjectGuid());
			if (Space == EMovieSceneObjectBindingSpace::Root){
				BindingID.ReinterpretAsFixed();
			}
			SequenceActor->AddBinding(BindingID, CameraActor);
			SequenceActor->SequencePlayer->Play();
		});
	});

	LoginSequencePlayFinisehd.AddLambda([this]() {
		StartupUIP = NewObject<UStartupUIP>();
		StartupUIP->Show();
	});
}

