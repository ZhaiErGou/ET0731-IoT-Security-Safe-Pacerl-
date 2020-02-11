"""
This is the python code that allows user to
use telegram bot to control the function of lock and unlock
"""

import json
import boto3
from botocore.vendored import requests

TELE_TOKEN='bot_chatId:xxxxxxxxxxxxxxxxxxxxxxxxxxx'
URL = "https://api.telegram.org/bot{}/".format(TELE_TOKEN)
client = boto3.client('iot-data', region_name='us-east-1') // generally , the region_name is "us-east-1"

MyID = xxxxxxxxx // your chat id 

def send_message(text, chat_id):
    if MyID == chat_id:
        if text == 'unlock':
            final_text = 'Box is Unlocked'
            # Change topic, qos and payload
            response = client.publish(
            topic='topic/servo',  // replace "topic/servo" subjected to the AWS IoT thing
            qos=0,
            payload=json.dumps(1)
            )
        
        elif text == 'lock':
            final_text = 'Box is locked'
            # Change topic, qos and payload
            response = client.publish(
            topic='topic/servo',  // replace "topic/servo" subjected to the AWS IoT thing
            qos=0,
            payload=json.dumps(0)
            )
        
        else:
            final_text = 'Error! Type "unlock" to unlock the box\n Type "lock" to lock the box'
    else:
        final_text = 'You are not allowed to use this bot'
    url = URL + "sendMessage?text={}&chat_id={}".format(final_text, chat_id)
    requests.get(url)

def lambda_handler(event, context):
    message = json.loads(event['body'])
    chat_id = message['message']['chat']['id']                 // get user id
    reply = message['message']['text']
    send_message(reply, chat_id)

    return {
        'statusCode': 200
    }
