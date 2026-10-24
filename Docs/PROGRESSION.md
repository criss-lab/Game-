# Progression integration

The offline USPProgressionSubsystem exposes CompleteMission, CollectPickup, PurchaseCar and EquipCar for Blueprint menus and mission completion logic. Rewards are claimed once per mission and pickups once per unique ID. Rank thresholds: 0 Recruit, 500 Operative, 2000 Specialist, 5000 Elite, 10000 Legend XP.

CarCatalog requires editor-configured offers with unique IDs, prices and required mission unlocks. Purchases record ownership and selection; actual vehicle spawn must resolve EquippedCar to a validated vehicle Blueprint. No vehicle meshes are included.

IMPORTANT: Existing SPProfileSubsystem uses a separate V1 save slot. Progression V2 uses its own slot; migration and unified save ownership are outstanding. CompleteMission must be called only after verified debrief in offline gameplay. Do not invoke it on scene entry or arbitrary UI button presses. Network rewards need authoritative server persistence and abuse protection.

UI: show coin counter, XP progress, rank, objectives, garage locked/unlocked states, purchase confirmation and insufficient balance errors. Mobile touch targets and safe area required. Test duplicate reward, pickup re-entry, save failure, corrupted data, purchase rollback and cold restart.
