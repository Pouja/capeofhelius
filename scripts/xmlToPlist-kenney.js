var fs = require('fs');
var minimist = require('minimist')(process.argv.slice(2));
var xml = fs.readFileSync(minimist.s + minimist.n + '.xml', 'utf8');
var _ = require('lodash');
var lines = xml.split('\n');
lines = lines.slice(1, lines.length - 1);

/**
 * Parses a single line in a xml, to extrac the width, height and coords in the sheet.
 * @param  {String} line A line in the xml.
 * @return {Object}      An object with all the information extracted.
 */
var parse = function(line) {
    var pattern = /name=\"(.*)\" x=\"(.*)\" y=\"(.*)\" width=\"(.*)\" height=\"(.*)\"\/\>/;
    var query = pattern.exec(line);
    return {
        name: query[1],
        width: parseInt(query[4]),
        height: parseInt(query[5]),
        x: parseInt(query[2]),
        y: parseInt(query[3])
    };
};

var data = lines.map(parse);

//////////////////////////////////////////////////////////////////////
// Below converts the array to plist which can be used by cocos2d-x //
//////////////////////////////////////////////////////////////////////

var header = '<?xml version="1.0" encoding="UTF-8"?>\n<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n<plist version="1.0">\n\t<dict>\n\t\t<key>frames</key>\n\t\t<dict>';
var footer = '\n\t</dict>\n</plist>';

/**
 * Makes a xml tag with a given tabsize infront of it.
 * @param  {Number} tabSize Number of tabs to lead.
 * @param  {String} prop    The property of the xml tag.
 * @param  {String} value   The value of the xml tag.
 * @return {String}         Xml tag as: <prop>value</prop>
 */
var makeTag = function(tabSize, prop, value) {
    var tabs = '';
    for (var i = 0; i < tabSize; i++) tabs += '\t';
    return tabs + '<' + prop + '>' + value + '</' + prop + '>';
}

/**
 * Makes a key string xml tag
 * @param  {Number} tabSize Number of tabs to lead.
 * @param  {String} keyValue     The key value
 * @param  {String} stringValue  The string value
 * @return {String}         Xml tag: <key>keyValue</key><string>stringValue</string>
 */
var makeKeyString = function(tabSize, keyValue, stringValue) {
    return [makeTag(tabSize, 'key', keyValue), makeTag(tabSize, 'string', stringValue)].join('\n');
}

/**
 * Makes a dict entry for the plist
 * @param  {Object} row The data for the dict.
 * @return {String}     An xml dict element.
 */
var makeEntry = function(row) {
    return [makeTag(3, 'key', row.name),
        '\t\t\t<dict>',
        makeKeyString(4, 'frame', '{{' + row.x + ',' + row.y + '},{' + row.width + ',' + row.height + '}}'),
        makeKeyString(4, 'offset', '{0,0}'),
        makeTag(4, 'key', 'rotated'),
        '\t\t\t\t<false/>',
        makeKeyString(4, 'sourceColorRect', '{{0,0},{' + row.width + ',' + row.height + '}}'),
        makeKeyString(4, 'sourceSize', '{' + row.width + ',' + row.height + '}'),
        '\t\t\t</dict>'
    ].join('\n');
};

/**
 * The metadata for cocos2d-x to be parsed.
 * @param  {String} png   The png file name.
 * @param  {Number} sizeX The x size of the png.
 * @param  {Number} sizeY The y size of the png.
 * @return {String}       Xml metadata.
 */
var makeMetaData = function(png, sizeX, sizeY) {
    return [makeTag(2, 'key', 'metadata'),
        '\t\t<dict>',
        makeKeyString(3, 'format', 2),
        makeKeyString(3, 'realTextureFileName', png),
        makeKeyString(3, 'size', '{' + sizeX + ',' + sizeY + '}'),
        makeKeyString(3, 'textureFileName', png),
        '\t\t</dict>'
    ].join('\n');
};

var xml = header + '\n' +data.map(makeEntry).join('\n') + '\t\t</dict>\n' + makeMetaData(minimist.n + '.png', 832, 1344) + '\n' +footer;
fs.writeFileSync(minimist.s + minimist.n + '.plist', xml, 'utf8');
