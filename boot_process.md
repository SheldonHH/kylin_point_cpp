# step 1: prepare config.ini and genesis.json

# step 2: run nodeos
su eosio

cd ~/pokemon
nodeos --config-dir config-dir/ -d data-dir/  --genesis-json config-dir/genesis.json

# step 3: prepare wallet
cleos wallet create
cleos wallet import <eosio-private-key>

# step 4: set contract eosio.bios
CONTRACTS_FOLDER='/home/bp01/eos/build/contracts'
cleos set contract eosio ${CONTRACTS_FOLDER}/eosio.bios -p eosio

# step 5: create system accounts
for account in eosio.bpay eosio.msig eosio.names eosio.ram eosio.ramfee eosio.saving eosio.stake eosio.token eosio.vpay
do
  echo -e "\n creating $account \n";
  cless create account eosio ${account} EOS8KCRvLd17A8dQr7HmAh1c1GksJNDYa6ZQpeE5zNhTz6k6MiiVS;
  sleep 1;
done

# step 6: set contract
cleos set contract eosio.token ${CONTRACTS_FOLDER}/eosio.token -p eosio.token
cleos set contract eosio.msig ${CONTRACTS_FOLDER}/eosio.msig -p eosio.msig

# step 7: create and issue token
cleos push action eosio.token create '["eosio", "10000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '["eosio",  "1000000000.0000 EOS", "cryptokylin"]' -p eosio

# setp 8: setting privileged account for eosio.msig
cleos push action eosio setpriv '{"account": "eosio.msig", "is_priv": 1}' -p eosio

# step 9: set contract eosio.system
cleos set contract eosio ${CONTRACTS_FOLDER}/eosio.system -x 1000 -p eosio
cgt eosio eosio rammarket | grep balance
cgt eosio eosio global | grep ram


cgt kylinspoints kylinspoints klpmarket | grep balance
cgt kylinspoints kylinspoints global | grep ram

sendklp '["hihelloeosgo","kylinspoints", "3000 KLP", "3000 KLP to kylinspoints"]' -p hihelloeosgo

# step 10: create some account
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eosecoeoseco EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eosbixincool EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eospaceioeos EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eosasia11111 EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eosstorebest EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eoslaomaocom EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eoshuobipool EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP

# --  faucet accounts -- 
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eosfaucet111 EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP
cleos system newaccount --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 EOS"  eosio eosio.faucet EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP EOS8TsBmmypMSzRRtNSMLCirvKfHYm7zmqcZGGvzYcxvX4ioQcDTP

# step 11: transfer
cleos  transfer  eosio  eosecoeoseco  "100000000.0000 EOS"
cleos  transfer  eosio  eosbixincool  "100000000.0000 EOS"
cleos  transfer  eosio  eospaceioeos  "100000000.0000 EOS"
cleos  transfer  eosio  eosasia11111  "100000000.0000 EOS"
cleos  transfer  eosio  eosstorebest  "100000000.0000 EOS"
cleos  transfer  eosio  eoslaomaocom  "100000000.0000 EOS"
cleos  transfer  eosio  eoshuobipool  "100000000.0000 EOS"


# step 12: transfer all eosio balance to faucet account and activate the network
cleos  transfer  eosio  eosfaucet111  "100000000.0000 EOS"
cleos  transfer  eosio  eosio.faucet  "199999730.0000 EOS"

## check eosio balance
cleos get currency balance eosio.token eosio


# step last: resign all system account
for account in eosio.bpay eosio.msig eosio.names eosio.ram eosio.ramfee eosio.saving eosio.stake eosio.token eosio.vpay
do
  cleos push action eosio updateauth '{"account": "'$account'", "permission": "active", "parent": "owner", "auth":{"threshold": 1, "keys": [], "waits": [], "accounts": [{"weight": 1, "permission": {"actor": "eosio", "permission": active}}]}}' -p ${account}@active
  cleos push action eosio updateauth '{"account": "'$account'", "permission": "owner", "parent": "",       "auth":{"threshold": 1, "keys": [], "waits": [], "accounts": [{"weight": 1, "permission": {"actor": "eosio", "permission": active}}]}}' -p ${account}@owner
  sleep 1;
done

## check system accounts
for account in eosio.bpay eosio.msig eosio.names eosio.ram eosio.ramfee eosio.saving eosio.stake eosio.token eosio.vpay
do
  echo --- ${account} --- && cleos get account ${account} && sleep 1;
done

cleos push action eosio updateauth '{"account": "eosio", "permission": "active", "parent": "owner", "auth":{"threshold": 1, "keys": [], "waits": [], "accounts": [{"weight": 1, "permission": {"actor": "eosio.prods", "permission": active}}]}}' -p eosio@active
cleos push action eosio updateauth '{"account": "eosio", "permission": "owner", "parent": "",       "auth":{"threshold": 1, "keys": [], "waits": [], "accounts": [{"weight": 1, "permission": {"actor": "eosio.prods", "permission": active}}]}}' -p eosio@owner

## check eosio
cleos get account eosio