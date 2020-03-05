/*
  ysGameObjectのマネージャー
*/
#pragma once
#include "gameObject/ysGameObject.h"
#include "util/Util.h"
	class ysGameObjectManager
	{
	public:
		ysGameObjectManager() {}
		~ysGameObjectManager() {}
		
		struct EffectTool
		{
			//Effekseerマネージャ管理。
			//インスタンスは1つ
			Effekseer::Manager* m_effekseerManager = nullptr;
			EffekseerRenderer::Renderer* m_effekseerRenderer = nullptr;

			//エフェクト
			Effekseer::Effect* m_sampleEffect = nullptr;
			Effekseer::Handle m_playEffectHandle = -1;
		};

		void InitEffekseer();
		//アップデート
		void Update();
		/// <summary>
		/// ゲームオブジェクトの名前キーを作成
		/// </summary>
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = Util::MakeHash("Undefined");
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = Util::MakeHash(objectName);
			}
			return hash;
		}

		//newGO
		template <class T>
		T* NewGameObject(const char* objectName)
		{
			T* newObj = new T;
			IGameObjectList.push_back(newObj);
			unsigned int hash = MakeGameObjectNameKey(objectName);
			newObj->m_nameKey = hash;
			return newObj;
		}

		//デリート
		void DeleteGOObject(IGameObject* go)
		{
			//リストから検索して、見つかったら削除する。
			for (auto it = IGameObjectList.begin();
				it != IGameObjectList.end();
				it++) {
				if ((*it) == go) {
					//削除リクエストを送る。
					go->RequestDelete();
				}
			}
		}

		/// <summary>
		/// ゲームオブジェクト名の検索。重い
		/// </summary>
		template<class T>
		T* FindGameObject(const char* objactName, bool enableErrorMessage)
		{
			unsigned int nameKey = Util::MakeHash(ObjectName);
			for (auto goList : IGameObjectList) {
				if (goList->m_nameKey == nameKey) {
					//発見
					T* p = dynamic_cast<T*>(go);
					if (p == nullptr && enableErrorMessage == true) {
						//型変換に失敗
						MessageBox("型変換に失敗しました");
					}
					return p;
				}
			}
		}

		template<class T>
		void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
		{
			unsigned int nameKey = Util::MakeHash(objectName);
			for (auto goList : IGameObjectList) {
				if (goList->m_nameKey == nameKey) {
					//見つけた。
					T* p = dynamic_cast<T*>(goList);
					if (func(p) == false) {
						//クエリ中断。
						return;
					}
				}
			}
		}
		//呼ばれたら1追加
		void Counting()
		{
			Count++;
		}
		int GetCount()
		{
			return Count;
		}
		//実行。ExecuteからDrawとUpdateに分離
		void Execute();
		//void Update();
		void Draw();
		//2dも追加
		void Render();
	private:
		//初期化。
		void Init(int gameObjectPropMax);

		void Start();
		//void Draw();
		void PostUpdate();
		void PostRender();

		void PreUpdate();
	private:
		//配列の数が変動したらカウントする変数を作りたかったので追加
		int Count = 0;
		
	private:
		//授業版格納庫
		std::vector<IGameObject*> IGameObjectList; //ゲームオブジェクトのリスト
		//配列の格納庫
		typedef std::list<IGameObject*> GameObjectList;
		//Listはつけない
		/// <summary>
		/// NewGO用のリスト。tkEngineと違い優先度はない。
		/// </summary>
		std::vector<GameObjectList> m_gameObjectListArray;
		/// <summary>
		/// DeleteGO用のリスト
		/// </summary>
		std::vector<GameObjectList> m_deleteObjectArray[2];
	};
	//外部アクセスをするので、extern宣言がいる。
	extern ysGameObjectManager g_goMgr;
	/// <summary>
	/// ゲームオブジェクト生成のヘルパー関数
	/// </summary>
	template<class T>
	static inline T* NewGO(const char* objectName)
	{
		return ysGameObjectManager().NewGameObject<T>(objectName);
	}
	/// <summary>
	/// ゲームオブジェクト削除のヘルパー関数
	/// NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行すること
	/// </summary>
	/// <param name="go">削除するゲームオブジェクト</param>
	static inline void DeleteGO(IGameObject* go)
	{
		g_goMgr.DeleteGOObject(go);
	}
	/// <summary>
	/// ゲームオブジェクト検索のヘルパー関数
	/// 同名のゲームオブジェクト全てに対してQueryを行いたい場合に使用する。
	/// objectName	ゲームオブジェクトの名前
	/// func		ゲームオブジェクトが見つかった時に呼ばれるコールバック関数。
	/// <param name="objectName"></param>
	/// <param name="func"></param>
	/// </summary>
	template<class T>
	static inline void QueryGOs(const char* objectName, std::function<bool(T* go)>func)
	{
		return g_goMgr.FindGameObjects<T>(objectName,func);
	}
	/// <summary>
	/// ゲームオブジェクトを名前指定で削除
	/// </summary>
	static inline void DeleteGOs(const char* objectName)
	{
		QueryGOs<IGameObject>(objectName, [](auto go) {
			DeleteGO(go);
			return true;
		});
	}
	extern ysGameObjectManager::EffectTool g_Effect;