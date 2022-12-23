//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//
//		基本プロシージャ・基本オブジェクトクラス処理
//																BaseProc.cpp
//=============================================================================
#include  "BaseProc.h"

//------------------------------------------------------------------------
//
//	基本プロシージャクラスのデストラクタ	
//  （全てのプロシージャやオブジェクトを読み下して削除する）
//
//  引数　なし
//
//------------------------------------------------------------------------
CBaseProc::~CBaseProc()
{
	SAFE_DELETE(m_pMesh);                       // メッシュの削除  // -- 2022.2.16

	// プロシージャポインタ配列の削除
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		SAFE_DELETE(m_pProcArray[i]);
	}
	// オブジェクトポインタ配列の削除
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		SAFE_DELETE(m_pObjArray[i]);
	}
}
//------------------------------------------------------------------------
//
//	基本プロシージャクラスの全体更新処理	
//  （全てのプロシージャやオブジェクトを読み下して更新処理をする）
//
//  引数　なし
//
//------------------------------------------------------------------------
void CBaseProc::UpdateAll()
{

	Update();                              // プロシージャの更新処理
	if (m_nWaitTime > 0) m_nWaitTime--;    // ウェイトタイマーのカウントダウン

	// オブジェクトポインタ配列の更新
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->Update();          // オブジェクトの更新処理
	}

	// 下位のプロシージャポインタ配列の更新
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->UpdateAll();      // 下位のプロシージャの全体更新処理（再帰処理）
	}

}
//-----------------------------------------------------------------------------
// 基本プロシージャクラスのあたり判定(移動直線)
//
// ・全ての相手プロシージャと相手オブジェクトを読み下し
// ・自オブジェクトのバウンディングボックスと相手の移動直線とのあたり判定処理をする
//
//   CBaseObj* pOtherObj    相手のオブジェクト
//   VECTOR3 vNow       相手の現在位置
//   VECTOR3 vOld       相手の一つ前の位置
//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//
//   判定結果　　
//           ヒットしたときは　　　　相手と自分のオブジェクトのm_pHitObjとm_vHitPosとm_vHitNormalに値をセット
//                                   相手と自分オブジェクトのm_dwStatusにDAMAGEを設定する
//           ヒットしていないときは　相手と自分のオブジェクトのm_pHitObjにNULLをセット
//
//-----------------------------------------------------------------------------
BOOL   CBaseProc::Hitcheck(CBaseObj* pOtherObj, VECTOR3 vNow, VECTOR3 vOld)
{
	BOOL  bRet = FALSE;

	// 全ての相手とのあたり判定
	for (DWORD i = 0; i < m_pObjArray.size(); i++)  // 全ての相手オブジェクト
	{
		bRet = m_pObjArray[i]->Hitcheck(pOtherObj, vNow, vOld);  // 相手オブジェクトとのあたり判定
		if (bRet) return bRet;
	}

	for (DWORD i = 0; i < m_pProcArray.size(); i++)  // 下位の相手プロシージャ
	{
		bRet = m_pProcArray[i]->Hitcheck(pOtherObj, vNow, vOld);  // 相手プロシージャとのあたり判定
		if (bRet) return bRet;
	}

	return bRet;
}

//-----------------------------------------------------------------------------
// 基本プロシージャクラスのあたり判定（バウンディングボックス）
//
// ・全ての相手プロシージャと相手オブジェクトを読み下し
// ・自オブジェクトのバウンディングボックスと相手の攻撃バウンディングボックスとのあたり判定
//
//   CBaseObj* pOtherObj      相手のオブジェクト
//   CBBox*    pAttackBBox    対象となるバウンディングボックス
//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//
//   判定結果　　
//           ヒットしたときは　　　　相手と自分のオブジェクトのm_pHitObjとm_vHitPosとm_vHitNormalに値をセット
//                                   相手と自分のオブジェクトのm_dwStatusにDAMAGEを設定する
//           ヒットしていないときは　相手と自分のオブジェクトのm_pHitObjにNULLをセット
//
//-----------------------------------------------------------------------------
BOOL   CBaseProc::Hitcheck(CBaseObj* pOtherObj, CBBox* pBBoxAtack)
{
	BOOL  bRet = FALSE;

	// 全ての相手とのあたり判定
	for (DWORD i = 0; i < m_pObjArray.size(); i++)  // 全ての相手オブジェクト
	{
		bRet = m_pObjArray[i]->Hitcheck(pOtherObj, pBBoxAtack);  // 相手オブジェクトとのあたり判定
		if (bRet) return bRet;
	}

	for (DWORD i = 0; i < m_pProcArray.size(); i++)  // 下位の相手プロシージャ
	{
		bRet = m_pProcArray[i]->Hitcheck(pOtherObj, pBBoxAtack);  // 相手プロシージャとのあたり判定
		if (bRet) return bRet;
	}

	return bRet;
}

//============================================================================
//   
//	プロシージャとオブジェクトを探索し全てのオブジェクトをノンアクティブにする
//   
//	引数　	なし
//
//	戻り値  なし
//============================================================================
void  CBaseProc::SetNonActive()
{

	// 敵のオブジェクトポインタ配列の探索
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		m_pObjArray[i]->SetActive(FALSE);   // アクティブフラグをFALSEに
		m_pObjArray[i]->ResetStatus();      // 各種ステータスをリセット
	}

	// 下位のプロシージャポインタ配列の探索
	for (DWORD i = 0; i < m_pProcArray.size(); i++)
	{
		m_pProcArray[i]->SetMaxWaitTime();    // ウェイトタイムをセットする
		m_pProcArray[i]->SetNonActive();      // 下位のプロシージャの処理（再帰処理）
	}
}


//------------------------------------------------------------------------  // -- 2022.2.16
//
//	基本オブジェクトクラスのコンストラクタ	
//
//  引数　CBaseProc* pProc   // 親のプロシージャ
//
//------------------------------------------------------------------------
CBaseObj::CBaseObj(CBaseProc* pProc)
{
	m_pGMain = pProc->GetGMain();		// ゲームメインクラス
	m_pProc  = pProc;					// 親のプロシージャ
	ResetStatus();						// 各種ステータスをリセット
	m_bActive = FALSE;					// TRUE:表示  FALSE:非表示
	m_dwObjID = pProc->GetProcID();		// オブジェクトＩＤ
	m_dwObjNo = pProc->GetObjArrayPtr().size();	// オブジェクトＮＯ  プッシュバック前の配列サイズがオブジェクトＮＯとなる
	m_pBBox = NULL;						// バウンディングボックスクラス
	m_mWorld = XMMatrixIdentity();		// ワールドマトリクス
	m_mWorldOld = XMMatrixIdentity();	// ワールドマトリクス（一つ前）
	m_vPosUp = VECTOR3(0, 0, 0);		// 移動増分
	m_vRotUp = VECTOR3(0, 0, 0);		// 回転増分
	m_fJumpY = 0.0f;					// ジャンプ高さ
	m_fJumpTime = 0.0f;					// ジャンプ時間
	m_pOyaObj = 0;						// 親オブジェクト（武器オブジェ等）
	m_pHitObj = 0;						// 当たった相手のオブジェクト
	m_vHitPos = VECTOR3(0, 0, 0);		// 当たった場所の座標
	m_vHitNormal = VECTOR3(0, 0, 0);	// 当たった場所の法線座標
	m_nCnt1 = 0;						// カウンター１
	m_nCnt2 = 0;						// カウンター２
	m_nMaxHp = 0;						// 最大体力
	m_nHp = 0;							// 体力
	m_nAtc = 0;							// 攻撃力

	m_AnimStatus.playAnim = true;		// アニメーションを動作させる
}
//------------------------------------------------------------------------
//
//	基本オブジェクトクラスのデストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CBaseObj::~CBaseObj()
{
	SAFE_DELETE(m_pBBox);
}
//============================================================================   // -- 2022.2.16
//   
//	親プロシージャのメッシュポインタのアドレスを返す
//   
//	引数　	なし
//
//	戻り値  メッシュポインタのアドレス
//============================================================================
CFbxMesh* CBaseObj::GetMesh()
{
	return m_pProc->GetMesh();
}
//============================================================================   // -- 2022.2.16
//   
//	親プロシージャの手に持つアイテムを返す
//   
//	引数　	なし
//
//	戻り値  手に持つアイテム
//============================================================================
BASEHOLDITEM CBaseObj::GetHoldItem()
{
	return m_pProc->GetHoldItem();
}

//-----------------------------------------------------------------------------
//
// 基本オブジェクトクラスのあたり判定(移動直線)
//
// ・自オブジェクトのバウンディングボックスと相手の移動直線とのあたり判定
//
//   CBaseObj* pOtherObj  相手のオブジェクト
//   VECTOR3 vNow     相手の現在位置
//   VECTOR3 vOld     相手の一つ前の位置
//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//
//   判定結果　　
//           ヒットしたときは　　　　相手と敵のオブジェクトのm_pHitObjとm_vHitPosとm_vHitNormalに値をセット
//                                   敵オブジェクトのm_dwStatusにDAMAGEを設定する
//           ヒットしていないときは　相手と敵のオブジェクトのm_pHitObjにNULLをセット
//
//-----------------------------------------------------------------------------
BOOL   CBaseObj::Hitcheck(CBaseObj* pOtherObj, VECTOR3 vNow, VECTOR3 vOld)
{
	BOOL bRet = FALSE;
	VECTOR3 vHit = VECTOR3(0.0f, 0.0f, 0.0f), vNrm = VECTOR3(0.0f, 1.0f, 0.0f);

	// バウンディングボックスと移動直線（Lay）による判定
	if ((m_bActive && m_dwStatus != DAMAGE && m_dwStatus != DEAD && m_dwStatus != FLASH && this != pOtherObj) &&
		m_pBBox->OBBCollisionLay(vNow, vOld, &vHit, &vNrm))
	{
		m_pHitObj = pOtherObj;
		pOtherObj->m_pHitObj = this;
		m_vHitPos = vHit;
		m_vHitNormal = vNrm;
		pOtherObj->m_vHitPos = vHit;
		pOtherObj->m_vHitNormal = vNrm;
		m_dwStatus = DAMAGE;
		pOtherObj->m_dwStatus = DAMAGE;
		bRet = TRUE;
	}
	return bRet;
}
//-----------------------------------------------------------------------------
//
// 基本オブジェクトクラスのあたり判定（バウンディングボックス）
//
// ・自オブジェクトのバウンディングボックスと相手のバウンディングボックスとのあたり判定
//
//   CBaseObj* pOtherObj  相手のオブジェクト
//   CBBox*      pBBoxAtack   バウンディングボックスの判定をするとき
//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//
//   判定結果　　
//           ヒットしたときは　　　　相手と敵のオブジェクトのm_pHitObjとm_vHitPosとm_vHitNormalに値をセット
//                                   敵オブジェクトのm_dwStatusにDAMAGEを設定する
//           ヒットしていないときは　相手と敵のオブジェクトのm_pHitObjにNULLをセット
//
//-----------------------------------------------------------------------------
BOOL   CBaseObj::Hitcheck(CBaseObj* pOtherObj, CBBox* pBBoxAtack)
{
	BOOL bRet = FALSE;
	VECTOR3 vHit = VECTOR3(0.0f, 0.0f, 0.0f), vNrm = VECTOR3(0.0f, 1.0f, 0.0f);

	// バウンディングボックス同士による判定
	if ((m_bActive && m_dwStatus != DAMAGE && m_dwStatus != DEAD && m_dwStatus != FLASH && this != pOtherObj) &&
		m_pBBox->OBBCollisionDetection(pBBoxAtack, &vHit, &vNrm))
	{
		m_pHitObj = pOtherObj;
		pOtherObj->m_pHitObj = this;
		m_vHitPos = vHit;
		m_vHitNormal = vNrm;
		pOtherObj->m_vHitPos = vHit;
		pOtherObj->m_vHitNormal = vNrm;
		m_dwStatus = DAMAGE;
		pOtherObj->m_dwStatus = DAMAGE;
		bRet = TRUE;
	}
	return bRet;
}

//-----------------------------------------------------------------------------
// 目的地（vTargetX,Z）への移動処理  汎用関数
//
//   引数
//               VECTOR3           :vTarget :行き先
//               float fSpeedIn    :一回の移動量
//               float fRotSpeed   :回転速度(省略値は3.0f)        // -- 2018.8.17
//
//   戻り値　：　TRUE：目的地に達した　　FALSE:まだ目的地に達していない
//               処理後、m_vPosUp, m_vRotUpに移動量が設定される
//-----------------------------------------------------------------------------
BOOL CBaseObj::TargetMove(VECTOR3 vTarget, float fSpeedIn, float fRotSpeed)
{
	BOOL ret = FALSE;
	VECTOR3 vMove, vObjPos, vObjPosOld;
	float fLen, fSpeed;
	const float fNearLimit = 0.1f;    // 目的地との近接リミット(10cm以内に近接したとき)

	vObjPos = GetPositionVector(m_mWorld);
	vObjPosOld = GetPositionVector(m_mWorldOld);

	vTarget.y = m_mWorld._42;		// ターゲットはオブジェクトと同一高さとする

	if ((vObjPos.x <= vTarget.x + fNearLimit && vObjPos.x >= vTarget.x - fNearLimit) &&		// 目的地に近接したとき
		(vObjPos.z <= vTarget.z + fNearLimit && vObjPos.z >= vTarget.z - fNearLimit))
	{  // 目的地に達したとき
		m_vPosUp.x = 0;
		m_vPosUp.y = 0;
		m_vPosUp.z = 0;
		m_vRotUp = VECTOR3(0, 0, 0);
		ret = TRUE; // 目的地に達した

	}
	else {    // 目的地への移動処理

		vMove = vTarget - vObjPos;
		fLen = magnitude(vMove);
		if (fLen < fSpeedIn)
		{
			fSpeed = fSpeedIn - fLen;
		}
		else {
			fSpeed = fSpeedIn;
		}

		m_vRotUp.y = GetTargetRotateVector(m_mWorld, vTarget).y;	// ターゲットの方向を向かせるY軸角度を求める
		if (m_vRotUp.y >= 180) m_vRotUp.y -= 360;
		if (m_vRotUp.y <= -180) m_vRotUp.y += 360;
		if (m_vRotUp.y >  fRotSpeed) m_vRotUp.y = fRotSpeed;		// fRotSpeed度以上の方向変換は行わない
		if (m_vRotUp.y < -fRotSpeed) m_vRotUp.y = -fRotSpeed;		// fRotSpeed度以上の方向変換は行わない

		m_vPosUp.x = 0;
		//m_vPosUp.y = -0.01f;		// 下方向へ重力
		m_vPosUp.y = 0;
		m_vPosUp.z = fSpeed;

		ret = FALSE;    // まだ目的地に達していない
	}

	return ret;
}
//-----------------------------------------------------------------------------  // -- 2018.8.2
// オブジェクトのmWorldと引数位置との距離を求める
//
//   引数　　：　vPos :位置
//   戻り値　：　距離
//-----------------------------------------------------------------------------
float CBaseObj::GetDistance(VECTOR3 vPos)
{
	VECTOR3 vVec;
	vVec = vPos - GetPositionVector(m_mWorld);

	return  magnitude(vVec);
}
//-----------------------------------------------------------------------------  // -- 2019.6.8
// ステータスをリセットする
//
//   引数　　：　なし
//   戻り値　：　なし
//-----------------------------------------------------------------------------
void CBaseObj::ResetStatus()
{
	m_dwStatus = NORMAL;			// ステータス
	m_dwStatusSub = ATTACKNONE;		// ステータスサブ
	m_dwStatusPhase = 0;			// ステータスフェーズ
}

//============================================================================
//  オブジェクト同士が重ならないようにするメンバ関数
//
//	引数　	CBaseProc* pProj　対象とする基本プロシージャ
//
//	戻り値CBaseObj* 	オブジェクトアドレス:強制移動の相手オブジェクト　NULL:移強制動していない
//============================================================================
CBaseObj*  CBaseObj::SetKeepOffset(CBaseProc* pProc)
{
	float	fMyRadius, fOtherRadius, fAddRadius, fLen, fMvMax = 0.0f;
	VECTOR3  vMy, vOther;
	VECTOR2  vMy2, vOther2;
	MATRIX4X4	 matWk1, matWk2;
	CBaseObj     *pOtherObj, *pTargetObj = NULL;
	CBBox		 *pOtherBBox;

	if (m_pBBox == NULL) return NULL;
	vMy = GetPositionVector(m_mWorld);
	vMy2.x = vMy.x;
	vMy2.y = vMy.z;

	// 自分の半径をバウンディングボックスから算出する（前と右幅の大きい方を採用）
	fMyRadius = (m_pBBox->m_vMax.x > m_pBBox->m_vMax.z) ? m_pBBox->m_vMax.x : m_pBBox->m_vMax.z;


	// オブジェクト配列の探索
	for (DWORD i = 0; i < pProc->GetObjArrayPtr().size(); i++)
	{
		pOtherObj = pProc->GetObjArrayPtr()[i];

		if (!pOtherObj->m_bActive) continue;	// 現在生きていなければ対象外
		if (pOtherObj == this) continue;	// 自分自身は対象外
		pOtherBBox = pOtherObj->m_pBBox;
		if (pOtherBBox == NULL) continue;	// バウンディングボックスがなければ対象外
		vOther = GetPositionVector(pOtherObj->m_mWorld);
		vOther2.x = vOther.x;
		vOther2.y = vOther.z;

		// バウンディングボックス同士の高さが一致しているか。範囲内に入っていなければ対象外
		if ((vMy.y + m_pBBox->m_vMin.y) > (vOther.y + pOtherObj->m_pBBox->m_vMax.y) ||
			(vMy.y + m_pBBox->m_vMax.y) < (vOther.y + pOtherObj->m_pBBox->m_vMin.y)) {
			continue;
		}

		// 相手の半径をバウンディングボックスから算出する（前と右幅の大きい方を採用）
		fOtherRadius = (pOtherBBox->m_vMax.x > pOtherBBox->m_vMax.z) ? pOtherBBox->m_vMax.x : pOtherBBox->m_vMax.z;

		fAddRadius = fMyRadius + fOtherRadius;
		if (fabs(vMy2.x - vOther2.x) < fAddRadius && fabs(vMy2.y - vOther2.y) < fAddRadius) {		// まず、ラフに水平方向（ＸＺ方向）で半径の範囲内に入っているか
			fLen = magnitude(vMy2 - vOther2);		// 水平方向（ＸＺ方向）で半径の距離内に入っているか
			if (fLen < fAddRadius) {
				fLen = fAddRadius - fLen;	// くい込み距離にする
				if (fLen > fMvMax) {		// 今までの中で、一番接近しているか
					fMvMax = fLen;
					pTargetObj = pOtherObj;
				}
			}
		}
	}

	if (pTargetObj) {	// 半径内に入った相手がいた場合
		vOther = GetPositionVector(pTargetObj->m_mWorld);
		vOther.y = vMy.y;
		matWk1 = GetLookatMatrix( vMy, vOther);					// 相手の方向を向かせるマトリックスを作成する
		matWk2 = XMMatrixTranslation(0.0f, 0.0f, -fMvMax);		// くい込み値だけ、Ｚ方向の反対方向に移動するマトリックスを作成する
		matWk1 = matWk2 * matWk1;								// くい込み値の反対方向に移動する
		vMy.x = 0.0f - vMy.x;
		vMy.y = 0.0f - vMy.y;
		vMy.z = 0.0f - vMy.z;
		matWk2 = XMMatrixTranslation(vMy.x, vMy.y, vMy.z);		// 自分の位置の逆数の位置
		matWk2 = m_mWorld * matWk2;								// 自分の原点位置での角度のみにする
		vMy = GetPositionVector(matWk1);						// くい込み値の反対方向に移動した位置
		matWk1 = XMMatrixTranslation(vMy.x, vMy.y, vMy.z);		// くい込み値の反対方向に移動した位置のみのマトリックス
		m_mWorld = matWk2 * matWk1;								// 自分の新しい位置と角度のワールドマトリックス
	}


	// 下位のプロシージャ配列の探索
	if (pTargetObj == NULL)
	{
		for (DWORD i = 0; i < pProc->GetProcArrayPtr().size(); i++)
		{
			pTargetObj = SetKeepOffset(pProc->GetProcArrayPtr()[i]);      // 下位のプロシージャの処理（再帰処理）
			if (pTargetObj) break;
		}
	}

	return pTargetObj;
}


