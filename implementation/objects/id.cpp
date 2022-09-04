class ID
{
protected:
    static int nextId;

public:
    int id;
    ID();
};

int ID::nextId = 0;

ID::ID()
{
    id = ++nextId;
}