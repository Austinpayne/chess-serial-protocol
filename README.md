### Chess Serial Protocol
`CMD [PARAMS]\n`
* CMD is cmd number (in hex)
* PARAMS depend on the command and are comma separated, i.e `p1,p2,p3,...,pN`
* Return status' should be `>= 0`, even for errors. Status' `< 0` are reserved for parsing errors.

### Need
* `0x0` status (params: `0=ok`)
* `0x1` start new game (params: `player_type=<human|ai>`)
* `0x2` end turn
* `0x3` move piece (params: `move` (in LAN format, ex. `e2e4`))
* `0x4` promote pawn (need to figure out how chess-js handles this server side)
* `0x5` calibrate (photon initiated on new game, probably don't need to have user access)
* `0x6` end game (photon --> touchscreen, could also be touchscreen --> photon to indicate player wants to leave game)

### Wow-factor
* `0x7` scan for wifi networks (see https://github.com/Austinpayne/photon-chess-client/issues/1)
* `0x8` set wifi

### Nice-to-have
* `0x9` get best move
* `0xA` query existing games
* `0xB` join existing game
