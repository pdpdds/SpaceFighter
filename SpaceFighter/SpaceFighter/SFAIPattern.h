#pragma once

class GameObject;

class SFAIPattern
{
public:
	SFAIPattern(void);
	virtual ~SFAIPattern(void);

	virtual void OnEnter(GameObject* pOwner);
	virtual void OnLeave();

	virtual BOOL ProcessAI(float fElapsedTime); 

	GameObject* GetOwner(){return m_pOwner;}
	BOOL GetActivation(){return m_bActivate;}

protected:
	float m_fTime;
	BOOL m_bMisaileFire;
private:
	GameObject* m_pOwner;
	BOOL m_bActivate;
	
};
