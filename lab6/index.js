let bmp = require('bmp-js');
let fs = require('fs');
let path = require('path');

function lsbBuf(buf) {
  return buf.map(i => (i & 1) * 0xff);
}

function lsbImage(inPath, outPath) {
  let image = bmp.decode(fs.readFileSync(inPath));
  image.data = lsbBuf(image.data);
  fs.writeFileSync(outPath, bmp.encode(image).data);
}

function main() {
  lsbImage('3_1.bmp', '3_1_out.bmp');
  lsbImage('3_2.bmp', '3_2_out.bmp');
  lsbImage('4_1.bmp', '4_1_out.bmp');
  lsbImage('4_2.bmp', '4_2_out.bmp');
  lsbImage('5_1.bmp', '5_1_out.bmp');
  lsbImage('5_2.bmp', '5_2_out.bmp');
}

main();
