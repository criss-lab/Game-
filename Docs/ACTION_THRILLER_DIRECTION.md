# Shadow Protocol: grounded action-thriller identity

Original fictional lead GHOST-01: economical dialogue, practical tactics, visible fatigue, close-range fight choreography, an uncompromising mission ethic. Do not reproduce a real performer's face, voice, identity, film dialogue or signature likeness.

Presentation integration: place SPThrillerPresentation in the level, bind the mission director, implement PresentBeat in a Blueprint widget to show chapter title, objective and subtitled radio line. Six default story beats are defined in C++. The mission remains playable if cinematic assets are absent. For mobile use compact typography, subtitles, low-cost transitions and safe-zone positioning.

Garage: place SPGarageSpawner and assign each CarId a licensed Chaos vehicle Blueprint with tuned wheels, controls and mobile LODs. Purchase and EquipCar are local offline progression operations; SpawnEquippedCar resolves the selected ID to the mapped class. This does not implement driving/possession or online vehicle authority.

HUD: create a widget Blueprint inheriting SPProgressionWidget. Implement RefreshProgression to update Coins, XP, Rank, IsOwned and EquippedCar. Bind BuyCar and SelectCar to confirmed button actions. Use OnProgressionUpdated rather than polling. Rank and garage visual assets must still be authored.

Verification gates: UE5.5 compile, functional level, sequence playback, vehicle physics and input, old-save migration, Android package and physical-device testing. Structural checks are not equivalent to these gates.
