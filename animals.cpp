class Animal
{
protected:
    int age;
    int maxAge;
    bool alive;

    // returns rabbit age.
    int getAge()
    {
        return age;
    }

    // ages the rabbit by 1 year.
    void increaseAge()
    {
        age++;
    }

    bool isAlive()
    {
        return alive;
    }

    bool setDead()
    {
        alive = false;
    }
};

class Rabbit : public Animal
{
private:
public:
};