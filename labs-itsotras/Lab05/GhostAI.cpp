#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"
#include "Math.h"

GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
	timeInCurrentState += deltaTime;
	float speed = 65.0f;
	if (mState == Scatter || mState == Chase) {
		speed = 90.0f;
	}
	else if (mState == Dead) {
		speed = 125.0f;
	}
	
	Vector2 pos = mGhost->GetPosition();

	pos += mDirection * speed * deltaTime;//set position

	mGhost->SetPosition(pos);

	if (mGhost->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>())) {
		mGhost->SetPosition(mNextNode->GetPosition());
		NeedToChangeState();

		UpdateTargetNode();
		UpdatePrevNextNodes();
		CalculateDirection();
	}

}

void GhostAI::Frighten()
{
	if (mState != Dead) {
		timeInCurrentState = 0.0f;
		if (mState != Frightened) {
			mState = Frightened;
			PathNode* temp = mNextNode;
			mNextNode = mPrevNode;
			mPrevNode = temp;
			mTargetNode = nullptr;
			CalculateDirection();
		}
	}
}

void GhostAI::Start(PathNode* startNode)
{
	timeInCurrentState = 0.0f;
	mGhost->SetPosition(startNode->GetPosition());
	mState = Scatter;
	mPrevNode = nullptr;
	mNextNode = startNode;
}

void GhostAI::Die()
{
	timeInCurrentState = 0.0f;
	mState = Dead;
	CalculateDirection();
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}
	
	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}
}

void GhostAI::UpdateTargetNode() {
	if (mState == Scatter) {
		mTargetNode = mGhost->GetScatterNode();
	}
	else if (mState == Frightened) {
		ChooseFrightenTarget();
	}
	else if (mState == Dead) {
		mTargetNode = mGhost->GetGame()->mGhostPen;
	}
	else {//chase state
		if (mGhost->GetType() == Ghost::Blinky) {
			PathNode* pacNode = mGhost->GetGame()->mPlayer->GetPrevNode();
			if (pacNode->GetType() == PathNode::Tunnel){
				Vector2 pacPos = mGhost->GetGame()->mPlayer->GetPosition();
				pacNode = GetClosestNodeToPoint(pacPos);

			}
			mTargetNode = pacNode;
		}
		else if (mGhost->GetType() == Ghost::Pinky) {
			Vector2 pos = mGhost->GetGame()->mPlayer->GetPointInFrontOf(80.0f);
			mTargetNode = GetClosestNodeToPoint(pos);
		}
		else if (mGhost->GetType() == Ghost::Inky) {
			Vector2 pointP = mGhost->GetGame()->mPlayer->GetPointInFrontOf(40.0f);
			Vector2 vectorV = pointP - mGhost->GetGame()->mGhosts[0]->GetPosition();
			Vector2 pointQ = mGhost->GetGame()->mGhosts[0]->GetPosition() + 2 * vectorV;
			mTargetNode = GetClosestNodeToPoint(pointQ);
		}
		else {//Clyde

			Vector2 pacPos = mGhost->GetGame()->mPlayer->GetPosition();
			if (Vector2::Distance(mGhost->GetPosition(), pacPos) >150.0f) {
				PathNode* pacNode = mGhost->GetGame()->mPlayer->GetPrevNode();
				if (pacNode->GetType() == PathNode::Tunnel) {
					pacNode = GetClosestNodeToPoint(pacPos);
				}
				mTargetNode = pacNode;
			}
			else {
				mTargetNode = mGhost->GetScatterNode();
			}
			
		}
	}
}

void GhostAI::UpdatePrevNextNodes() {
	float distance = -1.0f;
	PathNode* myNode = nullptr;
	Vector2 targetPos = mTargetNode->GetPosition();

	std::vector<PathNode*> possibleNodes;
	for (auto node : mNextNode->mAdjacent) {
		if (node != mPrevNode && node->GetType() == PathNode::Default) {
			possibleNodes.push_back(node);
		}
		if (mState == Dead) {
			if (node != mPrevNode && node->GetType() == PathNode::Ghost) {
				possibleNodes.push_back(node);
			}
		}
	}

	for (auto node: possibleNodes) {
		if (distance == -1.0f) {
			myNode = node;
			distance = Vector2::Distance(targetPos, node->GetPosition());

		}
		else if (Vector2::Distance(targetPos, node->GetPosition()) < distance) {
			myNode = node;
			distance = Vector2::Distance(targetPos, node->GetPosition());
		}
		
	}
	//if the first pass fails, add ghost nodes to the mix
	if (myNode == nullptr) {
		for (auto node : mNextNode->mAdjacent) {
			if (node != mPrevNode && node->GetType() == PathNode::Ghost) {//allow ghost nodes
				possibleNodes.push_back(node);
			}
		}
		for (auto node : possibleNodes) {
			if (distance == -1.0f) {
				myNode = node;
				distance = Vector2::Distance(targetPos, node->GetPosition());

			}
			else if (Vector2::Distance(targetPos, node->GetPosition()) < distance) {
				myNode = node;
				distance = Vector2::Distance(targetPos, node->GetPosition());
			}

		}
	}
	if (myNode == nullptr) {
		possibleNodes = mNextNode->mAdjacent;//allow all types
		for (auto node : possibleNodes) {
			if (distance == -1.0f) {
				myNode = node;
				distance = Vector2::Distance(targetPos, node->GetPosition());

			}
			else if (Vector2::Distance(targetPos, node->GetPosition()) < distance) {
				myNode = node;
				distance = Vector2::Distance(targetPos, node->GetPosition());
			}

		}
	}
	mPrevNode = mNextNode;
	mNextNode = myNode;
}

void GhostAI::CalculateDirection() {
	Vector2 myDirection = Vector2::Normalize(mNextNode->GetPosition() - mPrevNode->GetPosition());
	AnimatedSprite* mAnimator = mGhost->GetComponent<AnimatedSprite>();
	if (mState == Scatter || mState == Chase) {
		if (myDirection.x == -1.0f) {
			mAnimator->SetAnimation("left");
		}
		else if (myDirection.x == 1.0f) {
			mAnimator->SetAnimation("right");
		}
		else if (myDirection.y == 1.0f) {
			mAnimator->SetAnimation("down");
		}
		else {
			mAnimator->SetAnimation("up");
		}
	}
	else if(mState == Frightened) {
		if (timeInCurrentState > frightenTimer -2.0f) {
			mAnimator->SetAnimation("scared1");
		}
		else if (timeInCurrentState < frightenTimer - 2.0f) {
			mAnimator->SetAnimation("scared0");
		}
	}
	else {
		if (myDirection.x == -1.0f) {
			mAnimator->SetAnimation("deadleft");
		}
		else if (myDirection.x == 1.0f) {
			mAnimator->SetAnimation("deadright");
		}
		else if (myDirection.y == 1.0f) {
			mAnimator->SetAnimation("deaddown");
		}
		else {
			mAnimator->SetAnimation("deadup");
		}
	}
	
	mDirection = myDirection;
}

void GhostAI::ChooseFrightenTarget() {
	std::vector<PathNode*> possibleNodes;
	for (auto node : mNextNode->mAdjacent) {
		if (node != mPrevNode && node->GetType() == PathNode::Default) {
			possibleNodes.push_back(node);
		}
	}
	if (possibleNodes.empty()) {
		for (auto node : mNextNode->mAdjacent) {
			if (node != mPrevNode && node->GetType() != PathNode::Tunnel) {
				possibleNodes.push_back(node);
			}
		}
	}
	if (possibleNodes.empty()) {
		possibleNodes = mNextNode->mAdjacent;
	}

	Random rand;
	int index = rand.GetIntRange(0, (int)(possibleNodes.size())-1);
	mTargetNode = possibleNodes[index];
}

void GhostAI::NeedToChangeState() {
	if (mState == Frightened) {
		if (timeInCurrentState > frightenTimer) {
			mState = Scatter;
			timeInCurrentState = 0.0f;
		}
	}
	else if (mState == Dead) {
		if (mNextNode == mGhost->GetGame()->mGhostPen) {
			mState = Scatter;
			timeInCurrentState = 0.0f;
		}
	}
	else if (mState == Scatter) {
		if (timeInCurrentState > scatterTimer) {
			mState = Chase;
			timeInCurrentState = 0.0f;
		}
	}
	else{
		if (timeInCurrentState > chaseTimer) {
			mState = Scatter;
			timeInCurrentState = 0.0f;
		}
	}
}

PathNode* GhostAI::GetClosestNodeToPoint(Vector2 point) {
	float distance = -1.0f;
	PathNode* myNode;
	for (auto node : mGhost->GetGame()->mPathNodes) {
		if (node->GetType() == PathNode::Default) {
			if (distance == -1.0f) {
				myNode = node;
				distance = Vector2::Distance(point, node->GetPosition());
			}
			else if (Vector2::Distance(point, node->GetPosition()) < distance) {
				myNode = node;
				distance = Vector2::Distance(point, node->GetPosition());
			}
		}
	}
	return myNode;
}