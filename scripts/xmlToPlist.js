var fs = require('fs');
var minimist = require('minimist')(process.argv.slice(2));
var xml = fs.readFileSync(minimist.s + minimist.n + '.xml', 'utf8');
var _ = require('lodash');
var lines = xml.split('\n');
lines = lines.slice(3, lines.length - 3);

var parse = function(line) {
    var pattern = /\<cut w=\"(.*)\" x=\"(.*)\" y=\"(.*)\" h=\"(.*)\" row=\"(.*)\" col=\"(.*)\"\/\>/;
    var query = pattern.exec(line);
    return {
        width: parseInt(query[1]),
        height: parseInt(query[4]),
        x: parseInt(query[2]),
        y: parseInt(query[3])
    };
};

var data = lines.map(parse);

var metaData = [{
    count: 7,
    type: 'spell-front'
}, {
    count: 7,
    type: 'spell-left'
}, {
    count: 7,
    type: 'spell-front'
}, {
    count: 7,
    type: 'spell-right'
}, {
    count: 8,
    type: 'stab-front'
}, {
    count: 8,
    type: 'stab-left'
}, {
    count: 8,
    type: 'stab-front'
}, {
    count: 8,
    type: 'stab-right'
}, {
    count: 9,
    type: 'walk-front'
}, {
    count: 9,
    type: 'walk-left'
}, {
    count: 9,
    type: 'walk-front'
}, {
    count: 9,
    type: 'walk-right'
}, {
    count: 6,
    type: 'swing-front'
}, {
    count: 6,
    type: 'swing-left'
}, {
    count: 6,
    type: 'swing-front'
}, {
    count: 6,
    type: 'swing-right'
}, {
    count: 13,
    type: 'arrow-front'
}, {
    count: 13,
    type: 'arrow-left'
}, {
    count: 13,
    type: 'arrow-front'
}, {
    count: 13,
    type: 'arrow-right'
}, {
    count: 6,
    type: 'die'
}];

var makeName = function(type, index) {
    return minimist.n + '-' + type + '/' + index + '.png';
};

var addType = function(entries) {
    var entryIndex = 0;
    metaData.forEach(function(meta) {
        for (var index = 0; index < meta.count; index++) {
            entries[entryIndex].type = meta.type;
            entryIndex++;
        }
    });
};

var addName = function(entries){
    var index = 0;
    var current = entries[0].type;
    entries.forEach(function(entry){
        if(entry.type !== current) {
            current = entry.type;
            index = 0;
        }
        entry.name = makeName(entry.type, index);
        index++;
    });
}

addType(data);
data = _.sortByAll(data, ['type', 'x']);
addName(data);

var header = '<?xml version="1.0" encoding="UTF-8"?>\n<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n<plist version="1.0">\n\t<dict>\n\t\t<key>frames</key>\n\t\t<dict>';
var footer = '\n\t</dict>\n</plist>';

var makeTag = function(tabSize, prop, value) {
    var tabs = '';
    for (var i = 0; i < tabSize; i++) tabs += '\t';
    return tabs + '<' + prop + '>' + value + '</' + prop + '>';
}

var makeKeyString = function(tabSize, key, string) {
    return [makeTag(tabSize, 'key', key), makeTag(tabSize, 'string', string)].join('\n');
}

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