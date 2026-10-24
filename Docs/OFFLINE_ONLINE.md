# Offline and online play contract

The mode selection subsystem is a local preference and validation foundation, not a completed connection/session system.

| Mode | Offline | Online |
|---|---|---|
| Story campaign | Yes, local saves | Optional future co-op |
| Solo vs AI | Yes | Optional future |
| Private duel with selected consenting friend | No | Yes |
| Cops vs Thieves | AI team substitutes planned | Real teams planned |

Offline must never require login, persistent network connectivity or an online entitlement check after a legitimate local installation. Store campaign progression in local versioned save files. Online identity, friends, invitations, matchmaking, authoritative match state, anti-cheat and dedicated servers are future implementation work. Do not silently upload offline save data; ask for consent before optional cloud sync. Handle sync conflicts explicitly and preserve local saves. When internet disconnects during an online match, display reconnect/retry rather than falsely converting remote players into AI. LAN is a separate future option, not promised offline functionality.

UI flow: Start -> Offline / Online -> supported mode -> character/team/mission selection. Hide private duel in offline mode. Online mode requires a successful authenticated session before matchmaking. A local practice Cops vs Thieves match can use bots once bot AI and match objectives exist.

Acceptance: airplane-mode launch and local save/load; no authentication call on offline path; offline mode selection rejects private duel; online failure does not corrupt offline progress; dedicated-server tests for online modes; PC/Android packaged builds.
