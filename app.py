from flask import Flask, render_template, request, jsonify, send_file
import base64
from io import BytesIO

app = Flask(__name__)

def xor_cipher(data, key):
    key_bytes = key.encode()
    return bytes([b ^ key_bytes[i % len(key_bytes)] for i, b in enumerate(data)])

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/run', methods=['POST'])
def run():
    mode = request.form['mode']
    method = request.form['method']
    key = request.form['key']
    text = request.form.get('text', '')
    file = request.files.get('file')

    # 🔹 Handle File Input
    if file:
        file_data = file.read()
        filename = file.filename

        # XOR Cipher for any binary file
        if method == 'xor':
            result = xor_cipher(file_data, key)
            output_name = f"{'encrypted' if mode == 'encrypt' else 'decrypted'}_{filename}"

            # Return binary file directly for download
            return send_file(
                BytesIO(result),
                as_attachment=True,
                download_name=output_name,
                mimetype='application/octet-stream'
            )

    # 🔹 Handle Text (Caesar Cipher)
    if method == 'caesar':
        shift = int(key) % 26
        def caesar(s, shift_val):
            res = ''
            for ch in s:
                if ch.isalpha():
                    base = 'A' if ch.isupper() else 'a'
                    res += chr((ord(ch) - ord(base) + shift_val) % 26 + ord(base))
                else:
                    res += ch
            return res

        result = caesar(text, shift if mode == 'encrypt' else -shift)
        return jsonify({'output': result})

    # 🔹 Handle Text XOR Cipher  ✅ (added section)
    if method == 'xor' and text:
        result_bytes = xor_cipher(text.encode(), key)
        try:
            result = result_bytes.decode('utf-8')
        except:
            result = base64.b64encode(result_bytes).decode()
        return jsonify({'output': result})

    return jsonify({'output': '⚠️ Unsupported operation.'})

if __name__ == '__main__':
    app.run(debug=True)
