# JunoRig Proxy

This is an extremely high-performance proxy for the CryptoNote stratum protocol (including Monero and others).
It can efficiently manage over 100K connections on an inexpensive, low-memory virtual machine (with just 1024 MB of RAM).
The proxy significantly reduces the number of connections to the pool, decreasing 100,000 workers down to just 391 on the pool side.
The codebase is shared with the [XMRig](https://github.com/xmrig/xmrig) miner.

## Compatibility
Compatible with any pool and any miner that supports NiceHash.

## Why?
This proxy is designed to handle donation traffic from XMRig. No other solution works well with high connection and disconnection rates.

## Usage
:boom: If you are using Linux and need to manage over **1000 connections**, you must [increase the limits on open files](https://github.com/xmrig/xmrig-proxy/wiki/Ubuntu-setup).

## Juno Cash (rx/juno) Support

This fork adds support for Juno Cash mining using the rx/juno algorithm.

### Pool Mining

Connect to a Juno mining pool:

```bash
./junorig-proxy -o pool.example.com:3333 -u YOUR_WALLET_ADDRESS -p x -a rx/juno -b 0.0.0.0:3334 -m simple
```

Then point your [junorig](https://github.com/juno-cash/junorig) miners at the proxy:

```bash
./junorig -o 127.0.0.1:3334 -u x -p x -a rx/juno -m simple
```

### Solo/Daemon Mining

See the dedicated document in [doc/SOLO_MINING.md](doc/SOLO_MINING.md). 

For solo mining, the proxy connects directly to a Juno node's RPC interface:

```bash
./junorig-proxy -o 127.0.0.1:8232 --daemon -a rx/juno -u rpcuser -p rpcpass -b 0.0.0.0:3334 -m simple
```

**Important:** In daemon mode, `-u` and `-p` are RPC authentication credentials, not wallet addresses. The mining reward address is configured on the Juno daemon itself (via `-mineraddress` or config file).

#### Daemon Configuration

On the Juno node:
```bash
junocashd -server -rpcuser=rpcuser -rpcpassword=rpcpass -mineraddress=YOUR_WALLET_ADDRESS
```

Optional: Enable ZMQ for faster block notifications:
```bash
# On the daemon
junocashd -zmqpubhashblock=tcp://0.0.0.0:28332 ...

# On the proxy
./junorig-proxy -o 127.0.0.1:8232 --daemon --daemon-zmq-port=28332 -a rx/juno -b 0.0.0.0:3334
```

### Proxy Modes

- **simple**: All miners share the same job. Use `-m simple` if you have only one miner.
- **nicehash**: not supported

For rx/juno's 32-byte nonce, the proxy uses byte 7 for partitioning (supporting up to 256 concurrent miners per job).

