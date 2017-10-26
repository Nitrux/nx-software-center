#ifndef INTERACTOR_H
#define INTERACTOR_H


class Interactor
{
public:
    Interactor();
    virtual ~Interactor();
    virtual void execute() = 0;
};

#endif // INTERACTOR_H
