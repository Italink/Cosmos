#include "LoginGF.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Core/DynamicLambda.h"
#include "Kismet/GameplayStatics.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

void FLoginGF::Activate()
{
	Signals.WorldReady.BindEvent([this](UWorld* World) {
		LoadObjectAsync<ULevelSequence>(TEXT("/Game/Cosmos/Sequence/LS_Startup.LS_Startup"), [this, World](ULevelSequence* Sequence) {
			ACameraActor* CameraActor = World->SpawnActor<ACameraActor>();
			CameraActor->GetCameraComponent()->bConstrainAspectRatio = false;
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
			PlayerController->SetViewTarget(CameraActor);
			FMovieSceneSequencePlaybackSettings Settings;
			ALevelSequenceActor* SequenceActor = nullptr;
			ULevelSequencePlayer::CreateLevelSequencePlayer(World, Sequence, Settings, SequenceActor);
			SequenceActor->SequencePlayer->OnFinished.AddDynamicLambda(nullptr, [this]() {
				Signals.LoginSequencePlayFinisehd.Invoke();
			});
			UMovieScene* MovieScene = Sequence->GetMovieScene();
			check(MovieScene);
			const FMovieSceneBinding* Binding = Algo::FindBy(MovieScene->GetBindings(), FString("CameraActor"), &FMovieSceneBinding::GetName);
			check(Binding);
			EMovieSceneObjectBindingSpace Space = EMovieSceneObjectBindingSpace::Root;
			FMovieSceneObjectBindingID BindingID = UE::MovieScene::FRelativeObjectBindingID(Binding->GetObjectGuid());
			if (Space == EMovieSceneObjectBindingSpace::Root) {
				BindingID.ReinterpretAsFixed();
			}
			SequenceActor->AddBinding(BindingID, CameraActor);
			SequenceActor->SequencePlayer->Play();
		});
	});

	Signals.LoginSequencePlayFinisehd.BindEvent([this]() {
		StartupUIP = NewObject<UStartupUIP>();
		StartupUIP->Show();
	});
}
