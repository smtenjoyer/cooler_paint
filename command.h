#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand> //Используем класс, предоставляемый Qt
#include <QImage>

class DoodleArea; // Forward declaration

class Command : public QUndoCommand
{
public:
    Command(DoodleArea *area, const QString &text = QString());
    virtual ~Command() {}

protected:
    DoodleArea *doodleArea;
};

#endif // COMMAND_H

