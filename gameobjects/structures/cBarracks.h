// Structure class

class cBarracks : public cAbstractStructure {
private:


public:
    cBarracks();

    ~cBarracks();

    // overloaded functions    
    void think();

    void think_animation();

    void think_guard();

    void startAnimating() {};

    void draw() override { drawWithShadow(); }

    int getType() const;
};

