path = "C:\Program Files\Graphviz\bin";%path%

del socket_client_wav.gif
del socket_server_wav.gif
dot -Tgif socket_client_wav.dot -o socket_client_wav.gif
dot -Tgif socket_server_wav.dot -o socket_server_wav.gif

