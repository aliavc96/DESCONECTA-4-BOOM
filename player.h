#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
      double minimax(const Environment &T,int jugador, int & accion, int prof,double a,double b);
    private:
      int jugador_;
      Environment actual_;
};
#endif
