var nameMp = {
    '4piece_solved': 'n4_disconnected_letter',
    '4piece_square': 'n4_disconnected_square',

    '2piece-discon_solved': 'n2_disconnected_letter',
    '2piece-discon_square': 'n2_disconnected_square',

    '3piece_solved': 'n3_connected_letter',
    '3piece_square': 'n3_connected_square',

    '2piece_solved': 'n2_connected_letter',
    '2piece_square': 'n2_connected_square',
};

function printNode(node, str) {
    console.log("  // " + str);
    console.log("  {" + node["width"] + ", " + node["height"] + ", {");
    for (var j = 0; j < 36; j += 3) {
        process.stdout.write("    ");
        process.stdout.write("{" + node["pixels"][j] + "}, ");
        process.stdout.write("{" + node["pixels"][j+1] + "}, ");
        process.stdout.write("{" + node["pixels"][j+2] + "},");
        process.stdout.write("\n");
    }
    process.stdout.write("  }},\n");
}

for (const key in fonts) {
    console.log("const struct Font " + nameMp[key] + "[] = {");
    for (var i = 0; i < 10; ++i) {
        var node = fonts[key][i];
        printNode(node, i);
    }
    for (var i = 0; i < 26; ++i) {
        var x = String.fromCharCode(i + 65);
        var node = fonts[key][x];
        printNode(node, x);
    }
    if (key == '2piece_solved' || key == '2piece_square') {
        // do nothing
    } else {
        printNode(fonts[key]['&'], '&');
    }
    console.log("};");
}
