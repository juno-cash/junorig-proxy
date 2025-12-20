# Juno Cash Solo Mining Guide

This guide covers two solo mining setups:
- **Direct Mining**: Single miner connects directly to the Juno node
- **Proxy Mining**: Multiple miners connect through xmrig-proxy to a single Juno node

## Juno Node Setup

Configure your Juno node with RPC and ZMQ enabled.

`~/.junocash/junocashd.conf`:
```ini
server=1
rpcuser=YOUR_RPC_USER
rpcpassword=YOUR_RPC_PASSWORD
mineraddress=YOUR_JUNO_WALLET_ADDRESS
zmqpubhashblock=tcp://0.0.0.0:28332
```

| Setting | Purpose |
|---------|---------|
| `server=1` | Enables the RPC server |
| `rpcuser` / `rpcpassword` | RPC authentication credentials |
| `mineraddress` | Wallet address where block rewards are sent |
| `zmqpubhashblock` | ZMQ endpoint for instant block notifications (optional but recommended) |

Start the node:
```bash
junocashd
```

---

## Option A: Direct Mining (Single Miner)

```
[Juno Node:8232] <--RPC/ZMQ--> [juno-xmrig]
```

Best for: Single mining machine connecting directly to your node.

### Build juno-xmrig

```bash
git clone https://github.com/user/juno-xmrig
cd juno-xmrig
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Run

With ZMQ (recommended - instant block notifications):
```bash
./xmrig -o NODE_IP:8232 --daemon --daemon-zmq-port=28332 -a rx/juno -u RPC_USER -p RPC_PASSWORD
```

Without ZMQ (polls every 1000ms):
```bash
./xmrig -o NODE_IP:8232 --daemon -a rx/juno -u RPC_USER -p RPC_PASSWORD
```

Example:
```bash
./xmrig -o 192.168.0.10:8232 --daemon --daemon-zmq-port=28332 -a rx/juno -u m333 -p easypassword3
```

---

## Option B: Proxy Mining (Multiple Miners)

```
[Juno Node:8232] <--RPC/ZMQ--> [juno-xmrig-proxy:3334] <--stratum--> [miners...]
```

Best for: Mining farms, multiple machines, or when you want to hide RPC credentials from miners.

### Build juno-xmrig-proxy

```bash
git clone https://github.com/user/juno-xmrig-proxy
cd juno-xmrig-proxy
mkdir build && cd build
cmake .. -DWITH_TLS=ON
make -j$(nproc)
```

### Build juno-xmrig (for miners)

```bash
git clone https://github.com/user/juno-xmrig
cd juno-xmrig
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Run the Proxy

With ZMQ (recommended):
```bash
./xmrig-proxy -o NODE_IP:8232 --daemon --daemon-zmq-port=28332 -a rx/juno -u RPC_USER -p RPC_PASSWORD -b 0.0.0.0:3334
```

Without ZMQ:
```bash
./xmrig-proxy -o NODE_IP:8232 --daemon -a rx/juno -u RPC_USER -p RPC_PASSWORD -b 0.0.0.0:3334
```

Example:
```bash
./xmrig-proxy -o 192.168.0.10:8232 --daemon --daemon-zmq-port=28332 -a rx/juno -u m333 -p easypassword3 -b 0.0.0.0:3334
```

### Run Miners

Connect miners to the proxy (no RPC credentials needed):
```bash
./xmrig -o PROXY_IP:3334 -u worker1 -a rx/juno
```

Each miner can use any username - it's just for identification in proxy logs.

---

## Quick Reference

### Direct Mining Command
```bash
./xmrig -o NODE:8232 --daemon --daemon-zmq-port=28332 -a rx/juno -u USER -p PASS
```

### Proxy Command
```bash
./xmrig-proxy -o NODE:8232 --daemon --daemon-zmq-port=28332 -a rx/juno -u USER -p PASS -b 0.0.0.0:3334
```

### Miner to Proxy Command
```bash
./xmrig -o PROXY:3334 -u workername -a rx/juno
```

---

## Notes

- Block rewards go to the `mineraddress` configured in the Juno node
- ZMQ provides instant block notifications; without it, polling occurs every 1000ms
- The proxy supports up to 256 miners per job (nicehash mode)
- `-u`/`-p` on the proxy and direct mining are RPC credentials, not wallet addresses
- Miners connecting to the proxy use `-u` for worker identification only
