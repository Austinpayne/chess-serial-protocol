### Chess Serial Protocol
`CMD [PARAMS]\n`
* CMD is cmd number (in hex)
* PARAMS depend on the command and are comma separated, i.e `p1,p2,p3,...,pN`
* Return status' should be `>= 0`, even for errors. Status' `< 0` are reserved for parsing errors.

### Complete
* `0x0` status (params: `0=ok`)
* `0x1` start new game (params: `player_type=<human|ai>`)
* `0x2` end turn
* `0x3` move piece (params: `move,<c|p|k|q|e>?,<promo|extra_move|en_passant_capture>?` (in LAN format, ex. `0x3 e2e4`))
  * `c` capture, need to move piece at `move` first (ex. `0x3 e2e4,c`)
  * `p` promotion, need to move piece type `promo` to `move` (ex. `0x3 h7h8,p,q` promote to queen)
  * `k`/`q` castling, need to make `extra_move` as well (ex. `0x3 e1g1,k,h1f1` move rook at `h1` as well)
  * `e` en passant capture, need to remove `en_passant_capture` as well (ex. `0x3 b4a3,e,a4` remove `a4` as well)
* `0x4` user promote
* `0x5` calibrate (photon initiated on new game, probably don't need to have user access)
* `0x6` end game (photon --> touchscreen, could also be touchscreen --> photon to indicate player wants to leave game)
* `0x7` debug commands
* `0x9` send log (forward debug messages to photon)
* `0xA` end capture/castle
* `0xB` system reset
* `0xC` user turn ready (indicate to android that user can make moves)

### Incomplete (nice-to-have and wow-factor)
* `0x8` scan for wifi networks (see https://github.com/Austinpayne/photon-chess-client/issues/1)
* `0xD` set wifi
* `0xE` get best move
* `0xF` query existing games
* `0x10` join existing game
