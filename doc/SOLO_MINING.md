# Juno Cash Solo Mining Setup

## Architecture

```
[Juno Node:8232] <--RPC/ZMQ--> [xmrig-proxy:3334] <--stratum--> [miners]
```

juno-xmrig-proxy connects directly to the Juno node via RPC for solo mining.

## 1. Juno Node Setup

`~/.junocash/junocashd.conf`:
```ini
server=1
rpcuser=YOUR_RPC_USER
rpcpassword=YOUR_RPC_PASSWORD
mineraddress=YOUR_JUNO_WALLET_ADDRESS
zmqpubhashblock=tcp://127.0.0.1:28332
```

## 2. Stratum Proxy Setup

Build:
```bash
git clone https://github.com/user/juno-xmrig-proxy
cd juno-xmrig-proxy
mkdir build && cd build
cmake .. -DWITH_TLS=ON
make -j$(nproc)
```

Run:
```bash
./xmrig-proxy -o 127.0.0.1:8232 --daemon --daemon-zmq-port=28332 -a rx/juno -u YOUR_RPC_USER -p YOUR_RPC_PASSWORD -b 0.0.0.0:3334
```

## 3. Miner Setup

Build:
```bash
git clone https://github.com/user/juno-xmrig
cd juno-xmrig
mkdir build && cd build
cmake ..
make -j$(nproc)
```

Run:
```bash
./xmrig -o PROXY_IP:3334 -u worker1 -a rx/juno
```

## Notes

- Block rewards go to `mineraddress` configured on the Juno node
- `-u`/`-p` on the proxy are RPC credentials, not wallet addresses
- xmrig-proxy supports up to 256 miners per job (nicehash mode)
- ZMQ provides instant block notifications; without it, polling occurs every 1000ms

## Optional: RPC Proxy

For additional credential isolation on distributed setups, see [juno-proxy](https://github.com/user/juno-proxy).
