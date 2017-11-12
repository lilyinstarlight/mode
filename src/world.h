class World {
	public:
		World();
		virtual ~World() {}

		World(const World & w) = delete;
		const World & operator=(const World & w) = delete;

		void update();
		void draw() const;
	private:
};
