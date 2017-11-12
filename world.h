class World {
	public:
		World();
		virtual ~World() {}

		World() = delete;
		World(const World & v) = delete;
		const World & operator=(const World & v) = delete;

		void update();
		void draw() const;
	private:
};
