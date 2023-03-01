

export function Name() { return "HellowWord Keyboard"; }
export function VendorId() { return 0x1001; }
export function ProductId() { return 0xF103; }
export function Publisher() { return "HellowWord"; }
export function Size() { return [17, 8]; }
export function DefaultPosition(){return [75, 55]; }
export function DefaultScale(){return 18.8}
export function ControllableParameters() {
	return [
		{"property":"shutdownColor", "group":"lighting", "label":"Shutdown Color", "min":"0", "max":"360", "type":"color", "default":"009bde"},
		{"property":"LightingMode", "group":"lighting", "label":"Lighting Mode", "type":"combobox", "values":["Canvas", "Forced"], "default":"Canvas"},
		{"property":"forcedColor", "group":"lighting", "label":"Forced Color", "min":"0", "max":"360", "type":"color", "default":"009bde"},
	];
}
export function Initialize() {
	device.log("初始化")
}

const vKeys = 
[
    13,12,11,10,9,8,7,6,5,4,3,2,1,0,                          // 14
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,26, 27, 28, 
	43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,                    // 15
    //29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,40, 41, 42, 43,                     // 14
    44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,55, 56, 57, 
	71,70,69,68,67,66,65,64,63,62,61,60,59,58,                // 14
    //58,  59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,                 // 14
    72, 73, 74, 75, 76, 77, 78, 79, 80, 81, //10
	82, 83, 84, 
	//100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85 //背部16个灯
];


//LED对应Key的位置共82，还有19个为背景灯和三个指示灯
var vLedNames = [
	//"Pasue","f12","f11","f10","f9","f8","f7","f6","f5","f4","f3","f2","f1","esc",
	"Esc", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "Pasue",
	"~","1","2","3","4","5","6","7","8","9","0","-","=","backspace","Insert",
	"TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "LEFT_U_BRACE",  "RIGHT_U_BRACE", "BACKSLASH", "DELETE",
	"CAP_LOCK", "A","S","D","F","G","H","J","K","L","SEMI_COLON", "QUOTE", "ENTER", "PAGE_UP",
	"LEFT_SHIFT", "Z","X","C","V","B","N","M","COMMA", "PERIOD", "SLASH", "RIGHT_SHIFT", "UP_ARROW", "PAGE_DOWN",
	"LEFT_CTRL", "LEFT_GUI", "LEFT_ALT", "SPACE", "RIGHT_ALT", "FN", "RIGHT_CTRL", "LEFT_ARROW", "DOWN_ARROW", "RIGHT_ARROW",
	"Front1","Front2","Front3",
	//"back1","back2","back3","back4","back5","back6","back7","back8","back9","back10","back11","back12","back13","back14","back15","back16",
]
//上位机矩阵LED对应坐标点
var vLedPositions = [ 
	//键盘的82个灯
	[0,0],[1,0],[2,0],[3,0],[4,0],[5,0],[6,0],[7,0],[8,0],[9,0],[10,0],[11,0], [12,0], [13,0], //14
	//[13,0],[12,0],[11,0],[10,0],[9,0],[8,0],[7,0],[6,0],[5,0],[4,0],[3,0],[2,0], [1,0], [0,0], //14
	[0,1],[1,1],[2,1],[3,1],[4,1],[5,1],[6,1],[7,1],[8,1],[9,1],[10,1],[11,1], [12,1], [13,1], [14,1], //15
	[0,2],[1,2],[2,2],[3,2],[4,2],[5,2],[6,2],[7,2],[8,2],[9,2],[10,2],[11,2], [12,2], [13,2], [14,2] ,//15
	[0,3],[1,3],[2,3],[3,3],[4,3],[5,3],[6,3],[7,3],[8,3],[9,3],[10,3],[11,3], [12,3], [13,3], //14
	[0,4],[1,4],[2,4],[3,4],[4,4],[5,4],[6,4],[7,4],[8,4],[9,4],[10,4],[11,4], [12,4], [13,4], //14                   
	[0,5],[1,5],[2,5],[3,5],[4,5],[5,5],[6,5],[7,5],[8,5],[9,5], [10,5],[11,5],[12,5], //13 指示灯 3个 
	//背部16个灯
	//[0,6],[1,6],[2,6],[3,6],[4,6],[5,6],[6,6],[7,6],[8,6],[8,6],[10,6],[11,6], [12,6], [13,6], [14,6] ,[15,6],//16
]


export function LedNames() {
	return vLedNames;
}

export function LedPositions() {
	return vLedPositions;
}

export function Render() {
	sendColors();
}

export function Shutdown() {
	let packet = [];
		packet[0x00]   = 0x02;//reportid
		packet[0x01]   = 0xBD;//控制命令:关闭软件控制
		packet[0x02]   = 0;//第几包数据，共6包，120个灯
		packet[0x03]   = 1;//保留字节
		device.write(packet,32);
}
/**
	The three above examples are each suitable for a different packet structure. 
	The sendZone function is good for devices that use a separate packet for each LED. 
	The sendPacket function is more suitable for devices that send multiple LEDs in a row within a single packet. 
	The sendColors function is suitable for devices that have multiple LEDs in a packet, 
		but they are not one directly after the other. In our case, we are going to use the sendColors function. 
 * @param {*} zone 
 * @param {*} shutdown 
 */


function sendColors(shutdown = false){

	const RGBData = new Array(110*3).fill(0);
	for(let led_idx = 0; led_idx < vKeys.length; led_idx++) {
		let iX = vLedPositions[led_idx][0];
		let iY = vLedPositions[led_idx][1];
		var col;

		if(shutdown){
			col = hexToRgb(shutdownColor);
		}else if (LightingMode === "Forced") {
			col = hexToRgb(forcedColor);
			device.log("Forced："+col)
		}else{
			col = device.color(iX, iY);
		}
		let iLedIdx = vKeys[led_idx] * 3;
			RGBData[iLedIdx] = col[0];
			RGBData[iLedIdx + 1] = col[1];
			RGBData[iLedIdx + 2] = col[2];
	}
	
	//总共110灯珠，每次传输10个
	for(let ledPage = 0; ledPage < 12; ledPage++)
	{
		let packet = [];
		packet[0x00]   = 0x02;//reportid，不占用实际字节，由于是多id设备，必须占用
		packet[0x01]   = 0xac;//控制命令
		packet[0x02]   = ledPage;//包页码
		//3个字节对应一个rgb灯，一个包传10个，占用30字节
		packet = packet.concat(RGBData.splice(0, 30));
		device.write(packet,33);	
	}
}


function hexToRgb(hex) {
	let result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
	let colors = [];
	colors[0] = parseInt(result[1], 16);
	colors[1] = parseInt(result[2], 16);
	colors[2] = parseInt(result[3], 16);

	return colors;
}

export function Validate(endpoint) {
	return endpoint.interface === -1 && endpoint.usage === 0xc00 && endpoint.usage_page === 0xffc0;
}

export function Image() {
	return "";
}