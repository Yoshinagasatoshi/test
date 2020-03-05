#pragma once
/*
  ゲームオブジェクトの基底クラス。
*/
	class ysGameObjectManager;
	class IGameObject{
	public:
		using IGameObjectIsBase = IGameObject;
		//コンストラクタ
		IGameObject() :
			m_isStart(false),
			m_isDead(false)
		{}
		//デストラクタ
		virtual ~IGameObject()
		{

		}
		/// <summary>
			/// Updateの直前で呼ばれる開始処理
			/// </summary>
			/// <remarks>
			/// この関数が呼ばれるとゲームオブジェクトの準備が完了したと判断され、
			/// Update関数が呼ばれだします。trueを返して以降はstart関数は呼ばれません。
			/// ゲームオブジェクトの初期化に複数フレームかかる場合などはfalseを返して、初期化ステップ
			/// などを使って適切に初期化してください
			/// </remarks>
		virtual bool Start() { return true; }
		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update() {};
		/// <summary>
		/// Update関数が呼ばれる前に呼ばれる更新処理
		/// </summary>
		virtual void PostUpdate(){}
		/// <summary>
		/// Update関数が呼ばれた後に呼ばれる更新処理
		/// </summary>
		virtual void PreUpdate(){}
		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() {}
		/// <summary>
		/// ポストレンダ
		/// </summary>
		virtual void PostRender(){}
		/// <summary>
		/// レンダ
		/// </summary>
		virtual void Render(){}
		/// <summary>
		/// deletegoが実行されたときに呼ばれる
		/// </summary>
		virtual void OnDestroy() {};

		void StartWrapper()
		{
			if (m_isActive && !m_isStart && !m_isDead && !m_isRegist) {
				if (Start()) {
					//初期化処理が完了した。
					m_isStart = true;
				}
			}
		}
		void UpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegist) {
				Update();
			}
		}
		void PostUpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegist) {
				PostUpdate();
			}
		}
		void PreUpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegist) {
				PreUpdate();
			}
		}
		void DrawWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegist) {
				Draw();
			}
		}
		//殺してください関数。受理するだけで内容をすぐ消すわけではない。
		void RequestDelete()
		{
			m_isRequestdelete = true;
		}
		//死んでるかどうかを確認する関数。
		bool isRequestDelete()
		{
			return m_isRequestdelete;
		}
		friend class ysGameObjectManager;
	private:
	protected:
		bool m_isStart;                  //Start開始フラグ
		bool m_isDead;                   //死亡フラグ
		bool m_isRegistDeadList = false; //死亡リストに積まれている。
		bool m_isRegist = false;         //!<GameObjectManager>に登録されているかどうか
		bool m_isActive = true;          //Activeフラグ
		unsigned int m_nameKey = 0;		 //名前キー
		bool m_isRequestdelete = false;
	};