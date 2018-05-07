#define DEBUG
#include "GameObjects.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdio>

//GameObject
	GameObject::GameObject(){ };
	GameObject::~GameObject() { };
//GameObject

//Bullet
	Bullet::Bullet(GLint X, GLint Y, GLint Width, GLint Height) : ObjectType(BulletObject), Width(2), Height(2)
	// Bullet::Bullet() : ObjectType(BulletObject), Width(2), Height(2)
	{
		this->X=X;
		this->Y=Y;
		this->Width=Width;
		this->Height=Height;
		// printf("Bullet X: %d, Y: %d, Width: %d, Height: %d\n", X, Y, Width, Height);
	}
	Bullet::~Bullet()
	{
	}
//Bullet

//Tank
	Tank::Tank() : X(200), Y(200), ObjectType(TankObject)
	{ }

	Tank::~Tank() { }


	void Tank::moveForward(GLfloat Speed, GLint* map) 
	{ 
		if (BodyAngle==0.0f)
		{
			if (!isSolid(XCell+1, YCell, map))
			{
				X+=Speed;
			}
		}
		else if (BodyAngle==90.0f)
		{	
			if (!isSolid(XCell, YCell+1, map))
			{
				Y+=Speed;
			}	
		}
		else if (BodyAngle==180.0f){
			if (!isSolid(XCell-1, YCell, map))
			{
				X-=Speed;
			}
		}
		else if (BodyAngle==270.0f){
			if (!isSolid(XCell, YCell-1, map))
			{
				Y-=Speed;
			}
		}
	}
	void Tank::moveBack(GLfloat Speed, GLint* map) 
	{ 
		if (BodyAngle==0.0f)
		{
			if (!isSolid(XCell-1, YCell, map))
			{
				X-=Speed;
			}
		}
		else if (BodyAngle==90.0f)
		{	
			if (!isSolid(XCell, YCell-1, map))
			{
				Y-=Speed;
			}	
		}
		else if (BodyAngle==180.0f){
			if (!isSolid(XCell+1, YCell, map))
			{
				X+=Speed;
			}
		}
		else if (BodyAngle==270.0f){
			if (!isSolid(XCell, YCell+1, map))
			{
				Y+=Speed;
			}
		}
	}
	void Tank::rotateBody(GLfloat Angle) 
	{ 
		BodyAngle+=Angle;
		if (BodyAngle<0) BodyAngle+=360;
		else if (BodyAngle>=360) BodyAngle-=360;
	}
	void Tank::rotateTower(GLfloat Angle) 
	{ 
		TowerAngle=(TowerAngle+Angle);
		if (TowerAngle<0) TowerAngle+=360;
		else if (TowerAngle>=360) TowerAngle-=360;
	}
	int Tank::fire(std::vector<Bullet>& bullets, GLint* map) 
	{
		GLint BWidth = 2, BHeight = 2, BAngle = 0;
		GLint _x = XCell, _y = YCell;

		if (TowerAngle==0){
			while(*(map+20*YCell+(++_x))==0)
				{};
			if (*(map+20*YCell+_x)==1)
				BWidth=(_x-XCell)*CellSize-38;
			else BWidth=(_x-XCell)*CellSize-31;
			bullets.push_back(Bullet(X+38, Y+19, BWidth, 2));
		}
		else if (TowerAngle==180){
			while(*(map+20*YCell+(--_x))==0)
				{};
			if (*(map+20*YCell+_x)==1)
				BWidth=-(_x-XCell)*CellSize-38;
			else BWidth=-(_x-XCell)*CellSize-31;
			bullets.push_back(Bullet(X+2-BWidth, Y+19, BWidth, 2));
		}
		else if (TowerAngle==270){
			while(*(map+20*(--_y)+XCell)==0)
				{};
			if (*(map+20*_y+XCell)==1)
				BHeight=(YCell-_y)*CellSize-38;
			else BHeight=(YCell-_y)*CellSize-31;
			bullets.push_back(Bullet(X+19, Y+2-BHeight, 2, BHeight));
		}
		else if (TowerAngle==90){
			while(*(map+20*(++_y)+XCell)==0)
				{};
			if (*(map+20*_y+XCell)==1)
				BHeight=-(YCell-_y)*CellSize-38;
			else BHeight=-(YCell-_y)*CellSize-31;
			bullets.push_back(Bullet(X+19, Y+38, 2, BHeight));
		}
		return *(map+20*_y+_x);
	}
	void Tank::doAction(GLfloat dt, GLfloat TimeForAction, GLint* map){
		switch (action){
		case MoveForward:
			moveForward(CellSize*dt/TimeForAction, map);
			break;
		case MoveBack:
			moveBack(CellSize*dt/TimeForAction, map);
			break;
		case TurnBodyRight:
			rotateBody(90*dt/TimeForAction);
        	rotateTower(90*dt/TimeForAction);
			break;
		case TurnBodyLeft:
			rotateBody(-90*dt/TimeForAction);
        	rotateTower(-90*dt/TimeForAction);
			break;
		case TurnTowerRight:
			rotateTower(90*dt/TimeForAction);
			break;
		case TurnTowerLeft:
			rotateTower(-90*dt/TimeForAction);
			break;
		// case Fire:
		// 	fire();
		// 	break;
		}
	}	
	void Tank::think()
	{
		//
	}

	//Getters
	GLfloat Tank::getBodyAngle(){
		return BodyAngle;
	}
	GLfloat Tank::getTowerAngle(){
		return TowerAngle;
	}
	GLfloat Tank::getX(){
		return X;
	}
	GLfloat Tank::getY(){
		return Y;
	}
	GLint Tank::getXCell(){
		return (int)(getX()/CellSize);
	}
	GLint Tank::getYCell(){
		return (int)(getY()/CellSize);
	}
	GLint Tank::getID(){
		return this->ID;
	}
	Actions Tank::getAction(){
		return action;
	}
	//Setters
	void Tank::setBodyAngle(GLfloat newBodyAngle){
		BodyAngle = newBodyAngle;
	}
	void Tank::setTowerAngle(GLfloat newTowerAngle){
		TowerAngle = newTowerAngle;
	}
	void Tank::setX(GLfloat newX){
		X = newX;
	}
	void Tank::setY(GLfloat newY){
		Y = newY;
	}
	void Tank::setCell(GLint XCell, GLint YCell){
		setX(XCell*CellSize);
		setY(YCell*CellSize);
		setXCell(XCell);
		setYCell(YCell);
	}
	void Tank::setID(GLint ID){
		this->ID=ID;
	}
	void Tank::setXCell(GLint XC){
		XCell=XC;
		X=XCell*CellSize;
	}
	void Tank::setYCell(GLint YC){
		YCell=YC;
		Y=YCell*CellSize;
	}
	void Tank::setAction(Actions action){
		this->action=action;
	}
	void Tank::setAction(GLint action){
		switch (action){
		case 0:
			setAction(None);
			break;
		case 1:
			setAction(MoveForward);
			break;
		case 2:
			setAction(MoveBack);
			break;
		case 3:
			setAction(TurnBodyRight);
			break;
		case 4:
			setAction(TurnBodyLeft);
			break;
		case 5:
			setAction(TurnTowerRight);
			break;
		case 6:
			setAction(TurnTowerLeft);
			break;
		case 7:
			setAction(Fire);
			break;
		}
	}

	void Tank::checkData(GLint tankNumb, GLint* map)
	{
		*(map+20*YCell+XCell)=0;
		XCell=(int)((X+10)/CellSize);
		YCell=(int)((Y+10)/CellSize);
		X=XCell*CellSize;
		Y=YCell*CellSize;
		TowerAngle=round(TowerAngle);
		//BodyAngle=round(BodyAngle);
		if (BodyAngle<45 || BodyAngle>315) BodyAngle=0;
		else if (BodyAngle>45 && BodyAngle<135) BodyAngle=90;
		else if (BodyAngle>135 && BodyAngle<225) BodyAngle=180;
		else if (BodyAngle>225 && BodyAngle<315) BodyAngle=270;

		if (TowerAngle<45 || TowerAngle>315) TowerAngle=0;
		else if (TowerAngle>45 && TowerAngle<135) TowerAngle=90;
		else if (TowerAngle>135 && TowerAngle<225) TowerAngle=180;
		else if (TowerAngle>225 && TowerAngle<315) TowerAngle=270;

		*(map+20*YCell+XCell)=tankNumb;

		// std::cout<<"After checking data: " << std::endl << "XCell:YCell=" << XCell << ":" << YCell << std::endl
		// << "X:Y=" << X << ":" << Y << std::endl
		// << "BodyAngle=" << BodyAngle << std::endl
		// << "TowerAngle=" << TowerAngle << std::endl;
	}
	
	

	int Tank::isSolid(GLint XCell, GLint YCell, GLint* map){
	if (*(map+20*YCell+XCell)==0) return 0;
	return 1;
	}
//Tank