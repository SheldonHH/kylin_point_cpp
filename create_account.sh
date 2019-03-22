#!/bin/bash

#create random account
account=`./randon.sh`
echo $account



#随机生成account
for i in `seq 2`
do
    curl  http://faucet.cryptokylin.io/create_account?account |python -m json.tool|python -c "import json; import sys; obj=json.load(sys.stdin); print obj['keys']['active_key']['private'].encode('utf-8')"
done