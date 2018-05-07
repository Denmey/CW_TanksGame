#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <GL/glew.h>
#include <vector>

enum ObjectTypes{
	TankObject,
	WallObject,
	BulletObject
};
enum Actions{
	None,
	MoveForward,
	MoveBack,
	TurnBodyRight,
	TurnBodyLeft,
	TurnTowerRight,
	TurnTowerLeft,
	Fire
};

const int CellSize = 40;

class GameObject{
public:
	GameObject();
	~GameObject();
	ObjectTypes ObjectType;
};

class Bullet : GameObject
{
public:
	Bullet(GLint X, GLint Y, GLint Width, GLint Height);
	// Bullet();
	~Bullet();

	ObjectTypes ObjectType;
	GLint X, Y, Width, Height;
};

class Tank : GameObject{
public:
	//Constructor
	Tank();
	//Deconstructor
	~Tank();

	void setAction(Actions action);
	void setAction(GLint action);
	void doAction(GLfloat dt, GLfloat TimeForAction, GLint* map);
	void rotateBody(GLfloat Angle);
	void moveForward(GLfloat Speed, GLint* map);
	void moveBack(GLfloat Speed, GLint* map);
	void checkData(GLint tankNumb, GLint* map);
	void rotateTower(GLfloat Angle);
	int fire(std::vector<Bullet>& bullets, GLint* map);
	void think();
	GLfloat getBodyAngle();
	GLfloat getTowerAngle();
	GLfloat getX();
	GLfloat getY();
	Actions getAction();
	GLint getXCell();
	GLint getYCell();
	GLint getID();
	void setBodyAngle(GLfloat newBodyAngle);
	void setTowerAngle(GLfloat newTowerAngle);
	void setX(GLfloat newX);
	void setY(GLfloat newY);
	void setCell(GLint XCell, GLint YCell);
	void setXCell(GLint XC);
	void setYCell(GLint YC);
	void setID(GLint ID);
	int isSolid(GLint XCell, GLint YCell, GLint* map);


private:
	GLfloat BodyAngle, TowerAngle, X, Y;
	GLint ID, XCell, YCell;
	ObjectTypes ObjectType;
	Actions action;
	static int Count;
};

#endif