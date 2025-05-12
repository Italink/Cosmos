#include "SignalEventManager.h"
#include "Kismet/GameplayStatics.h"

USignalEventManager* USignalEventManager::Get(UObject* InWorldContext /*= nullptr*/)
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
	return GameInstacne->GetSubsystem<USignalEventManager>();
}

bool USignalEventManager::RemoveSignalEvent(int32 ID)
{
	if (auto IDInfoPtr = EventIDMap.Find(ID)){
		if (auto EventArrayPtr = EventNameMap.Find(IDInfoPtr->Key)){
			for (auto Event : *EventArrayPtr){
				if (Event.Get() == IDInfoPtr->Value){
					EventIDMap.Remove(ID);
					EventArrayPtr->Remove(Event);
					return true;
				}
			}
		}
	}
	return false;
}

void USignalEventManager::CleanupSignalEvents(FName SignalName)
{
	if (auto EventArrayPtr = EventNameMap.Find(SignalName)) {
		for (auto Event : *EventArrayPtr) {
			if (Event) {
				EventIDMap.Remove(Event->GetID());
				EventArrayPtr->Remove(Event);
			}
		}
	}
}