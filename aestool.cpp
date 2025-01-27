#include <gtkmm.h>
#include <cstdlib>
#include <string>
#include <fstream>

void executeCommand(const std::string& command) {
    std::system(command.c_str());
}

class EncryptionApp : public Gtk::Window {
public:
    EncryptionApp() {
        set_title("AES-Tool");
        set_default_size(500, 250);

        // main container
        mainBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        mainBox.set_spacing(20);
        add(mainBox);

        // encryption
        encryptionBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
        encryptionBox.set_spacing(5);
        encryptionBox.set_border_width(5);
        encryptionText.set_placeholder_text("message...");
        encryptButton.set_label("Encrypt");
        encryptButton.signal_clicked().connect(sigc::mem_fun(*this, &EncryptionApp::on_encrypt_button_clicked));
        encryptionBox.pack_start(encryptionText, Gtk::PACK_SHRINK);
        encryptionBox.pack_start(encryptButton, Gtk::PACK_SHRINK);

        // decryption
        decryptionBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
        decryptionBox.set_spacing(5);
        decryptionBox.set_border_width(5);
        encryptedFileInput.set_placeholder_text("encrypted file path...");
        decryptButton.set_label("Decrypt");
        decryptButton.signal_clicked().connect(sigc::mem_fun(*this, &EncryptionApp::on_decrypt_button_clicked));
        decryptionBox.pack_start(encryptedFileInput, Gtk::PACK_SHRINK);
        decryptionBox.pack_start(decryptButton, Gtk::PACK_SHRINK);

        // output box
        outputText.set_editable(false);

        // pack everything into mainbox
        mainBox.pack_start(encryptionBox, Gtk::PACK_SHRINK);
        mainBox.pack_start(decryptionBox, Gtk::PACK_SHRINK);
        mainBox.pack_start(outputText, Gtk::PACK_EXPAND_WIDGET);

        show_all_children();
    }

protected:
    void on_encrypt_button_clicked() {
        std::string input = encryptionText.get_text();
        if (input.empty()) {
            outputText.get_buffer()->set_text("Enter text to encrypt");
            return;
        }
        const std::string encryptedFile = "Encrypted.gpg";
        executeCommand("echo \"" + input + "\" | gpg --symmetric --cipher-algo AES256 --output " + encryptedFile);
        outputText.get_buffer()->set_text("Encrypted to: " + encryptedFile);
    }

    void on_decrypt_button_clicked() {
        std::string encryptedFile = encryptedFileInput.get_text();
        if (encryptedFile.empty()) {
            outputText.get_buffer()->set_text("Enter the encrypted file path");
            return;
        }
        const std::string decryptedFile = "decrypted.txt";
        executeCommand("gpg --decrypt --output " + decryptedFile + " " + encryptedFile);

        // read decrypted message
        std::ifstream file(decryptedFile);
        if (file.is_open()) {
            std::string decryptedText((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            outputText.get_buffer()->set_text("decrypted:\n" + decryptedText);
        } else {
            outputText.get_buffer()->set_text("error: failed to decrypt the file.");
        }
    }

private:
    Gtk::Box mainBox{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Box encryptionBox{Gtk::ORIENTATION_VERTICAL};
    Gtk::Box decryptionBox{Gtk::ORIENTATION_VERTICAL};
    Gtk::Entry encryptionText, encryptedFileInput;
    Gtk::Button encryptButton, decryptButton;
    Gtk::TextView outputText;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.noface.aestool");
    EncryptionApp window;
    return app->run(window);
}
