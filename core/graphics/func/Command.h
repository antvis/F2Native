#ifndef XG_GRAPHICS_FUNC_COMMAND_H
#define XG_GRAPHICS_FUNC_COMMAND_H

namespace xg {
namespace func {
struct Command {
    virtual void run() {}

    virtual ~Command() {}
};

template <typename Function> struct CommandImpl : Command {
    CommandImpl(Function _func) : func(_func) {}
    virtual void run() override { func(); }
    Function func;
};

template <typename Function, typename... ArgTypes> Command *CreateCommand(Function f) { return new CommandImpl<Function>(f); }

} // namespace func
} // namespace xg

#endif // XG_GRAPHICS_FUNC_COMMAND_H
