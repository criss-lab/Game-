# Progression and garage integration

The offline USPProgressionSubsystem is the single source of truth for alias, mission unlocks, completion count, coins, XP, ranks, pickup claims, car ownership and equipped-car selection. SPProfileSubsystem remains as a compatibility facade and delegates to that subsystem; it no longer owns a second save slot.

The subsystem loads ShadowProtocolProgressV2 on game-instance initialization. If that slot does not exist, it attempts the legacy ShadowProtocolLocalV1 save migration. Existing mission progress is preserved without retroactively granting rewards. New profiles receive the Street Compact as a free owned starter car.

Mission rewards are granted by SPMissionRewards on the Debrief scene in standalone/offline play. A mission reward and a pickup ID can only be claimed once. CompleteMission and CollectPickup persist transactionally and roll back in-memory changes if saving fails. Network rewards are intentionally disabled pending authoritative server validation.

Ranks are based on XP: Recruit (0), Operative (500), Specialist (2,000), Elite (5,000), Legend (10,000). CarCatalog has Street Compact (250), City Sedan (800), Tactical SUV (1,800), Sport Coupe (3,500) offers. The Street Compact is also provided free as the starter, so it should render as Owned rather than purchasable. RequiredMission means the mission number currently available to the player; it is compared with UnlockedMission.

Garage Blueprints must map each CarId to a configured ASPDriveableVehicle subclass. SPGarageSpawner only spawns an owned equipped car in standalone mode. Vehicle possession, tuned wheel physics and vehicle assets still need editor configuration.

Build a widget Blueprint from SPProgressionWidget and implement RefreshProgression for rank, XP, coins, owned status, and purchase/equip results. Add safe-area-aware touch controls for Android. Source sanity checks verify structural contracts only; UE5.5 compilation, gameplay tests, vehicle handling, visual UI, Android packaging, and device tests remain separate release gates.
