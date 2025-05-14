#include "command.h"
#include "doodlearea.h"

Command::Command(DoodleArea *area, const QString &text)
    : QUndoCommand(text), doodleArea(area)
{
}
