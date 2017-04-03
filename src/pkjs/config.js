module.exports = [
  {
    "type": "heading",
    "defaultValue": "GridHR+ Configuration"
  },
     {
        "type": "text",
        "defaultValue": "<h6>A white button = OFF, an orange button = ON</h6>",
      },
	{
    "type": "section",
    "items": [		
	{
        "type": "heading",
        "defaultValue": "<h6>Display</h6>"
	},	
 	{
  "type": "radiogroup",
  "messageKey": "row2",
//  "defaultValue": 0,
  "label": "Row 2 show",
  "options": [
    { 
      "label": "Day Name",
      "value": 0
    },
    { 
      "label": "Week #",
      "value": 1
    },
	      { 
      "label": "Battery %",
      "value": 2
    }
  ]
},	
		 	{
  "type": "radiogroup",
  "messageKey": "row3",
//  "defaultValue": 0,
  "label": "Row 3 show",
  "options": [
    { 
      "label": "Day #",
      "value": 0
    },
    { 
      "label": "Temperature",
      "value": 1
    }
  ]
},	
  {
    "type": "section",
    "items": [	
 	  {
        "type": "heading",
        "defaultValue": "<h5>Vibration</h5>"
      },
      {
        "type": "toggle",
        "messageKey": "bluetoothvibe",
        "label": "Bluetooth Vibration",
        "defaultValue": false
      },
	  {
        "type": "toggle",
        "messageKey": "hourlyvibe",
        "label": "Vibrate each hour",
        "defaultValue": false
      },
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Temperature Format"
      },
 
      {
        "type": "toggle",
        "messageKey": "WeatherIsFahrenheit",
        "label": "Celcius (off), Fahrenheit (on)",
        "defaultValue": true
      }
    ]
  },
]
  },

  {
        "type": "text",
        "defaultValue": "<h6>Please consider making a <a href='https://www.paypal.me/markchopsreed'>small donation here</a>, If you find this watchface useful. Thankyou.</h6>",
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];