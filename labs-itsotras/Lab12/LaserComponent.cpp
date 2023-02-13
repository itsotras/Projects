#include "LaserComponent.h"
#include "Shader.h"
#include "Actor.h"
#include "Texture.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Game.h"
#include "Renderer.h"
#include "SegmentCast.h"
#include "Block.h"
#include "Player.h"
#include "PlayerMove.h"

LaserComponent::LaserComponent(Actor* owner) :MeshComponent(owner, true) {
	SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Draw(Shader* shader) {
	if (mMesh)
	{
		for (auto myLine : mySegments) {
			// Set the world transform
			shader->SetMatrixUniform("uWorldTransform", GetMatrixFromSegment(myLine));
			// Set the active texture
			Texture* t = mMesh->GetTexture(mTextureIndex);
			if (t)
			{
				t->SetActive();
			}
			// Set the mesh's vertex array as active
			VertexArray* va = mMesh->GetVertexArray();
			va->SetActive();
			// Draw
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}
	}
}
void LaserComponent::Update(float deltaTime) {
	mySegments.clear();
	LineSegment myLine;
	myLine.mStart = mOwner->GetWorldPosition();
	myLine.mEnd = mOwner->GetWorldPosition() + 500.0f * mOwner->GetWorldForward();
	CastInfo myInfo;
	Vector3 newDir = mOwner->GetWorldForward();
	Block* myBlock = (Block*)mOwner->GetParent();
	while (true) {
		myLine.mEnd = myLine.mStart + 500.0f * newDir;
		if (SegmentCast((Actor*)mOwner->GetGame()->GetPlayer(), myLine, myInfo)) {//I interact with player
			myLine.mEnd = myInfo.mPoint;
			mySegments.push_back(myLine);
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
			mOwner->GetGame()->GetPlayer()->GetComponent<PlayerMove>()->Respawn();
			break;
		}
		else if (SegmentCast(mOwner->GetGame()->getBlocks(), myLine, myInfo, myBlock)) {//I interact with a block
			myLine.mEnd = myInfo.mPoint;
			myBlock = (Block*)(myInfo.mActor);

			if (myBlock->GetMirror()) {//I interact with a mirror block
				Vector3 oldDir = Vector3::Normalize(myLine.mEnd - myLine.mStart);
				mySegments.push_back(myLine);
				myLine.mStart = myInfo.mPoint;
				newDir = Vector3::Reflect(oldDir, myInfo.mNormal);
			}
			else {
				mySegments.push_back(myLine);
				break;
			}
		}
		else { //I interact with nothing
			mySegments.push_back(myLine);
			break;
		}
	}
}

Matrix4 LaserComponent::GetMatrixFromSegment(LineSegment line) {
	Matrix4 scale = Matrix4::CreateScale(line.Length(), 1.0f, 1.0f);
	Vector3 newDir = Vector3::Normalize(line.mEnd - line.mStart);
	Vector3 oldDir = Vector3::UnitX;
	float dot = Vector3::Dot(newDir, oldDir);
	Quaternion myQuat;
	if (dot == 1.0f) {
		myQuat = Quaternion::Identity;
	}
	else if (dot == -1.0f) {
		myQuat = Quaternion(Vector3::UnitZ, Math::Pi);
	}
	else {
		float theta = Math::Acos(dot);
		Vector3 axis = Vector3::Cross(oldDir, newDir);
		axis = Vector3::Normalize(axis);
		myQuat = Quaternion(axis, theta);
	}
	Matrix4 rotation = Matrix4::CreateFromQuaternion(myQuat);
	Matrix4 translation = Matrix4::CreateTranslation(line.PointOnSegment(.5));
	return scale * rotation * translation;
}