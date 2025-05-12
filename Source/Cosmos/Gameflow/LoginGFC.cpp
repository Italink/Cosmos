#include "LoginGFC.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

void ULoginGFC::Activate()
{
	Signals.WorldReady.BindEvent([this](UWorld* World) {
		LoadObjectAsync<ULevelSequence>(TEXT("/Game/Cosmos/Sequence/LS_Startup.LS_Startup"), [this, World](ULevelSequence* Sequence) {
			SequenceCameraActor = World->SpawnActor<ACameraActor>();
			SequenceCameraActor->GetCameraComponent()->bConstrainAspectRatio = false;
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
			PlayerController->SetViewTarget(SequenceCameraActor);

			FMovieSceneSequencePlaybackSettings Settings;
			ALevelSequenceActor* SequenceActor = nullptr;
			ULevelSequencePlayer::CreateLevelSequencePlayer(World, Sequence, Settings, SequenceActor);
			SequenceActor->SequencePlayer->OnFinished.AddDynamicLambda(nullptr, [this]() {
				Signals.OpeningSequencePlayFinisehd.Invoke();
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
			SequenceActor->AddBinding(BindingID, SequenceCameraActor);
			SequenceActor->SequencePlayer->Play();
		});

	});

	Signals.OpeningSequencePlayFinisehd.BindEvent([this]() {
		TSharedPtr<FStreamableHandle> ShowMenuSequenceStreamableHandle = LoadObjectAsync<ULevelSequence>(TEXT("/Game/Cosmos/Sequence/LS_Startup.LS_Startup"));
		StartupUIP = NewObject<UStartupUIP>();
		StartupUIP->AddToRoot();
		StartupUIP->Show();
		StartupUIP->View->Button_Start->OnClicked.AddDynamicLambda(StartupUIP, ([ShowMenuSequenceStreamableHandleRaw=ShowMenuSequenceStreamableHandle.Get(), this]() {
			ShowMenuSequenceStreamableHandleRaw->WaitUntilComplete();
			ULevelSequence* Sequence = ShowMenuSequenceStreamableHandleRaw->GetLoadedAsset<ULevelSequence>();
			FMovieSceneSequencePlaybackSettings Settings;
			ALevelSequenceActor* SequenceActor = nullptr;
			ULevelSequencePlayer::CreateLevelSequencePlayer(SequenceCameraActor->GetWorld(), Sequence, Settings, SequenceActor);
			SequenceActor->SequencePlayer->OnFinished.AddDynamicLambda(nullptr, [this]() {
				Signals.ShowMenuSequencePlayFinisehd.Invoke();
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
			SequenceActor->AddBinding(BindingID, SequenceCameraActor);
			SequenceActor->SequencePlayer->Play();
		}));
	});

	Signals.ShowMenuSequencePlayFinisehd.BindEvent([this]() {
		UGameflowContextManager::Get()->Pop<ULoginGFC>();
	});
}
