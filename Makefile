CPP_IN=smart_account_creator
PK=EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
CLEOS=$(CLEOS)
CONTRACT_ACCOUNT=sac123451235
EOS_CONTRACTS_DIR=/Users/al/Projects/eos/build/contracts
ACCOUNT_NAME := $(shell python gen.py)
CLEOS=cleos
NONCE := $(shell openssl rand 8 -hex)
HASH := $(shell /bin/echo -n '$(ACCOUNT_NAME)$(NONCE)'| shasum -a256 | awk '{print $$1}')

build:
	eosiocpp -o $(CPP_IN).wast $(CPP_IN).cpp

abi:
	eosiocpp -g $(CPP_IN).abi $(CPP_IN).cpp

all: build abi

deploy: build
	$(CLEOS) set contract $(CONTRACT_ACCOUNT) ../smart_account_creator

system:
	$(CLEOS) create account eosio angelo $(PK) $(PK)
	$(CLEOS) create account eosio eosio.token $(PK) $(PK)
	$(CLEOS) create account eosio eosio.msig $(PK) $(PK)
	$(CLEOS) create account eosio eosio.bpay $(PK) $(PK)
	$(CLEOS) create account eosio eosio.names $(PK) $(PK)
	$(CLEOS) create account eosio eosio.ram $(PK) $(PK)
	$(CLEOS) create account eosio eosio.ramfee $(PK) $(PK)
	$(CLEOS) create account eosio eosio.saving $(PK) $(PK)
	$(CLEOS) create account eosio eosio.stake $(PK) $(PK)
	$(CLEOS) create account eosio eosio.vpay $(PK) $(PK)
	$(CLEOS) set contract eosio.token $(EOS_CONTRACTS_DIR)/eosio.token -p eosio.token
	$(CLEOS) set contract eosio.msig $(EOS_CONTRACTS_DIR)/eosio.msig -p eosio.msig
	$(CLEOS) push action eosio.token create '["eosio", "10000000000.0000 EOS",0,0,0]' -p eosio.token
	$(CLEOS) push action eosio.token issue '["eosio","1000000000.0000 EOS", "issue"]' -p eosio
	$(CLEOS) transfer eosio angelo "1000.0000 EOS"
	$(CLEOS) set contract eosio $(EOS_CONTRACTS_DIR)/eosio.system -p eosio


setup:
	$(CLEOS) system newaccount --stake-net "1.0000 EOS" --stake-cpu "1.0000 EOS" --buy-ram-kbytes 8000 eosio $(CONTRACT_ACCOUNT) $(PK) $(PK)
	$(CLEOS) set account permission $(CONTRACT_ACCOUNT) active '{"threshold": 1,"keys": [{"key": "$(PK)","weight": 1}],"accounts": [{"permission":{"actor":"$(CONTRACT_ACCOUNT)","permission":"eosio.code"},"weight":1}]}' owner -p $(CONTRACT_ACCOUNT)
	$(CLEOS) system newaccount --stake-net "1.0000 EOS" --stake-cpu "1.0000 EOS" --buy-ram-kbytes 8000 eosio saccountfees $(PK) $(PK)

test:
	$(CLEOS) transfer angelo $(CONTRACT_ACCOUNT) "1.0000 EOS" "$(ACCOUNT_NAME):EOS7R6HoUvevAtoLqUMSix74x9Wk4ig75tA538HaGXLFKpquKCPkH:EOS6bWFTECWtssKrHQVrkKKf68EydHNyr1ujv23KCZMFUxqwcGqC3" -p $(CONTRACT_ACCOUNT)@active -p angelo@active
	$(CLEOS) get account $(ACCOUNT_NAME)
	

testbinance:
	$(CLEOS) push action $(CONTRACT_ACCOUNT) regaccount '["angelo", "$(HASH)", "$(NONCE)", "$(PK)", "$(PK)"]' -p angelo
	$(CLEOS) transfer angelo $(CONTRACT_ACCOUNT) "1.0000 EOS" "$(ACCOUNT_NAME)$(NONCE)" -p angelo
	$(CLEOS) get account $(ACCOUNT_NAME)

	
delete:
	$(CLEOS) push action $(CONTRACT_ACCOUNT) clearall '["$(CONTRACT_ACCOUNT)"]' -p $(CONTRACT_ACCOUNT)
	
	
show:
	$(CLEOS) get table $(CONTRACT_ACCOUNT) $(CONTRACT_ACCOUNT) order
	
clean:
	rm -f $(CPP_IN).wast $(CPP_IN).wasm
