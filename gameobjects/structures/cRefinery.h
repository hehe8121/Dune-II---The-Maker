// Structure class

class cRefinery : public cAbstractStructure
{
private:
    int iFade;          // Fading progress (windtraps)
    bool bFadeDir;      // Fading direction (TRUE -> up, FALSE -> down)

    // TIMER
    int TIMER_fade;

	void think_harvester_deploy();

public:
    cRefinery();
    ~cRefinery();

    // overloaded functions
    void think();
    void think_animation();
    void think_guard();
    void think_unit_occupation();
    void startAnimating() {};
    void draw() override { drawWithShadow(); }

    int getType() const;
	int getSpiceSiloCapacity();

};

