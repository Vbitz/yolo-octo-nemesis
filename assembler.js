var fs = require("fs");

var BLOCK_SIZE = 32;

var specialBlocks = {
	"main": 0
};

function assemble(lines, cb) {
	var buf =  new Buffer(256);
	buf.fill(0x00);
	var blocks = {};
	var nextBlock = BLOCK_SIZE; // the main block is reserved
	var currentPos = 0;
	for (var i = 0; i < lines.length; i++) {
		var line = lines[i];
		if (line.indexOf(":") === line.length) {
			var blockName = line.substring(0, line.length - 1);
			var blockPos = nextBlock;
			if (specialBlocks[blockName] !== undefined) {
				blockPos = specialBlocks[blockName];
			} else {
				nextBlock += BLOCK_SIZE;
			}
			blocks[blockName] = blockPos;
			currentPos = blockPos;
		} else if (line.indexOf("\t") === 0) {
			var tokens = line.substring(1).split(" ");
			if (tokens[0] === "hlt") {
				buf.writeUInt8(0x02, currentPos);
				currentPos++;
			} else if (tokens[0] === "brk") {
				buf.writeUInt8(0x03, currentPos);
				currentPos++;
			} else if (tokens[0] === "mov") {
				if (tokens[1].indexOf("$") === 0) {
					buf.writeUInt8(0x11, currentPos);
					buf.writeUInt8(parseInt(tokens[1].substring(1), 10), currentPos + 1);
					buf.writeUInt8(parseInt(tokens[2].substring(1), 10), currentPos + 2);
					currentPos += 3;
				} else {
					buf.writeUInt8(0x10, currentPos);
					buf.writeUInt16LE(parseInt(tokens[1], 10), currentPos + 1);
					buf.writeUInt8(parseInt(tokens[2].substring(1), 10), currentPos + 3);
					currentPos += 4;
				}
			} else if (tokens[0] === "add") {
				buf.writeUInt8(0x20, currentPos);
				buf.writeUInt8(parseInt(tokens[1].substring(1), 10), currentPos + 1);
				buf.writeUInt8(parseInt(tokens[2].substring(1), 10), currentPos + 2);
				currentPos += 3;
			} else if (tokens[0] === "sub") {
				buf.writeUInt8(0x21, currentPos);
				buf.writeUInt8(parseInt(tokens[1].substring(1), 10), currentPos + 1);
				buf.writeUInt8(parseInt(tokens[2].substring(1), 10), currentPos + 2);
				currentPos += 3;
			} else {
				cb(new Error("invalid instruction"), null);
			}
		} else if (line.trim.length === 0) { // skip empty lines
		} else if (line.indexOf("#") === 0) { // skip comment lines
		} else {
			cb(new Error("invalid instruction"), null);
		}
	}
	cb(undefined, buf);
}

function main(args) {
	if (args[2] === undefined) {
		console.log("error : usage : node assember.js [filename]");
	}
	var filename = args[2];
	console.log("assembling: " + filename);
	fs.readFile(filename, "utf8", function (err, buf) {
		if (err) throw err;
		assemble(buf.split('\n'), function (err, buf) {
			if (err) throw err;
			fs.writeFile(filename.substring(0, filename.lastIndexOf(".")) + ".bin", buf, function (err) {
				if (err) throw err;
				console.log("finished");
			});
		});
	});
}

if (module !== undefined) {
	module.exports = assemble;
	if (require.main === module) {
		main(process.argv);
	}
}