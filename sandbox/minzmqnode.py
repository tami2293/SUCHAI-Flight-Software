import zmq
import time
import argparse
from threading import Thread

def monitor(port="8002", ip="localhost", node=b''):
    """ Read messages from node(s) """
    if node != b'':
        node = chr(int(node)).encode("ascii", "replace")
    ctx = zmq.Context(1)
    sock = ctx.socket(zmq.SUB)
    sock.setsockopt(zmq.SUBSCRIBE, node)
    sock.connect('tcp://{}:{}'.format(ip, port))
    
    while True:
        frame = sock.recv_multipart()[0]
        print('MON:', frame)

def console(port="8001", ip="localhost"):
    """ Send messages to node """
    ctx = zmq.Context(1)
    sock = ctx.socket(zmq.PUB)
    sock.connect('tcp://{}:{}'.format(ip, port))
    prompt = "[node({})] <message>: "
    node = 1
    
    while True:
        try:
            cmd = input(prompt.format(node)).split(" ", 1)
            if len(cmd) > 1:
                node = int(cmd[0])
                data = cmd[1]
            else:
                data = cmd[0]

            if len(data) > 0:
                msg = (chr(node) + data).encode("ascii","replace")
                #print(msg)
                sock.send(msg)
        except Exception:
            pass
        time.sleep(0.25)

def get_parameters():
    """ Parse command line parameters """
    parser = argparse.ArgumentParser()

    parser.add_argument("-n", "--node", default=b'', help="Node address")
    parser.add_argument("-d", "--ip", default="localhost", help="Hub IP address")
    parser.add_argument("-i", "--in_port", default="8001", help="Input port")
    parser.add_argument("-o", "--out_port", default="8002", help="Output port")
    parser.add_argument("--nmon", action="store_false", help="Disable monitor task") 
    parser.add_argument("--ncon", action="store_false", help="Disable console task")

    return parser.parse_args()

if __name__ == "__main__":
    # Get arguments
    args = get_parameters()
    print(args)
    
    tasks = []

    if args.nmon:
        # Start monitor thread
        mon_th = Thread(target=monitor, args=(args.out_port, args.ip, args.node))
        mon_th.daemon = True
        tasks.append(mon_th)
        mon_th.start()

    if args.ncon:
        # Create a console socket
        con_th = Thread(target=console, args=(args.in_port, args.ip))
        con_th.daemon = True
        tasks.append(con_th)
        con_th.start()
        
    for th in tasks:
        th.join()

