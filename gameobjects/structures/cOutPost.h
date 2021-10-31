// Structure class

class cOutPost : public cAbstractStructure
{
private:
  // outpost does not use any variables in p

public:
    cOutPost();
    ~cOutPost();

    // overloaded functions
    void think();
    void think_animation();
    void think_guard();
    void think_flag();
    void startAnimating() {};

    int getType() const;

};

