# SDL-PPONG

![ppong.png](ppong.png)

```shell
~/SDL-PPONG/player1> make prepare
~/SDL-PPONG/player1> make player1
~/SDL-PPONG/player1> ./player1 --help
```

Sun RPC use case
================

SunRPC mechanism is used to:

1. Exchange paddle positions between players.
2. Launch the ball by the winner player, at the side of the second player, to start a new round. 

The [player1_rpc.x](https://github.com/MagonBC/SDL-PPONG/blob/main/player1/rpc/player1_rpc.x) and [player2_rpc.x](https://github.com/MagonBC/SDL-PPONG/blob/main/player2/rpc/player2_rpc.x) files
are used to generate client and server stubs for each player:

```shell
~/SDL-PPONG/player1/rpc> rpcgen -aM player1_rpc.x 
~/SDL-PPONG/player1/rpc> ls
player1_rpc_clnt.c  player1_rpc.h  player1_rpc_svc.c  player1_rpc.x ...
```

You don't need to regenerate stubs.

Playing with the computer
=========================

We don't yet implement this feature properly. Maybe we will use ML (We think on [tensorflow](https://www.tensorflow.org/install/lang_c)) to do it in the future!

Currently, if player2 is not connected, player1 can play the role of the computer by using keys 'x' and 'v' to move paddle2 :p

Nice!
