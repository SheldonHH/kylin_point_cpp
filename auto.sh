#!/bin/bash
echo "$*"
#create random account
account=`./randon.sh`
echo $account



#随机生成account
for i in `seq 2`
do
    curl  http://faucet.cryptokylin.io/create_account?account |python -m json.tool|python -c "import json; import sys; obj=json.load(sys.stdin); print obj['keys']['active_key']['private'].encode('utf-8')"
done



TOKENCREATOR=$1
CONNECTOR=$2
C_PUBLIC_KEY=$3
T_PRIK=$4
C_PRIK=$5

impt
expect: {
    "private key: " {send T_PRIK}
}
impt
expect: {
    "private key: " {send C_PRIK}
}

cssb eoshenzhensa TOKENCREATOR '10 EOS'
cssb eoshenzhensa CONNECTOR '10 EOS'

csc TOKENCREATOR ~/pp/build/klp.token
cless set account permission kylinstokens active '{"threshold": 1,"keys": [{"key": "EOS7EWMm39hbX4wyqWeqN3aeQS6cNC896dgke5BKRSpsgJhG2qbbV","weight": 1}],"accounts": [{"permission":{"actor":"kylinstokens","permission":"eosio.code"},"weight":1}]}' owner -p kylinstokens

cpa TOKENCREATOR create '["CONNECTOR","68719476736 KLP"]' -p TOKENCREATOR
cpa TOKENCREATOR issue '["CONNECTOR","68719476736 KLP"， "issue all token to TOKENCREATOR"]' -p CONNECTOR

csc CONNECTOR ~/pp/build/klp.connector
cgcb TOKENCREATOR CONNECTOR KLP
cgt CONNECTOR CONNECTOR klpmarket



