-module(porti).
-compile([export_all]).

-define(PORT_NAME,   "porti").


control(Port, Data) ->
    Port ! {self(), {command, Data}},
    receive
        {Port, {data, AnswerData}} ->
            AnswerData;
        {Port, {exit_status, _Status}} ->
            erlang:error(port_exit)
    end.
            

open() ->
    Opts = [{packet, 4}, binary, exit_status, use_stdio],
    erlang:open_port({spawn_executable, ?PORT_NAME}, Opts).
