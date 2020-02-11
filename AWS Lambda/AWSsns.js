var AWS = require("aws-sdk");
var X ="Your Parcel has been open";

exports.handler = function(event, context) {
    var eventText = JSON.stringify(event, null, 2);
    console.log("Received event:", eventText);
    var Open = (JSON.parse(eventText)).state.reported.Open;
    
    if (Open==1) {
        var sns = new AWS.SNS({region: 'us-east-1'});
        var params = {
            Message:  X , 
            TopicArn: "arn:aws:sns:us-east-1:173208927255:OTP"
        };}
    else{
        return 0;
    };
    sns.publish(params, context.done);
};
