import torch.nn as nn
from ConvLSTM import ConvLSTM


class SimpleCNN(nn.Module):
    def __init__(self):
        super(SimpleCNN, self).__init__()
        self.conv_layers = nn.Sequential(
            nn.Conv2d(1, 32, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),  # First pooling layer
            nn.Conv2d(32, 64, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            # Reeduce feature map size
            nn.Conv2d(64, 128, kernel_size=3, stride=2, padding=1),
            nn.ReLU(),
            # Remove the third pooling layer
        )

    def forward(self, x):
        return self.conv_layers(x)


class CNNLSTMModel(nn.Module):
    def __init__(self):
        super(CNNLSTMModel, self).__init__()
        self.encoder = SimpleCNN()

        # Assuming the output of SimpleCNN is a 128-dimensional feature map
        self.conv_lstm = ConvLSTM(
            input_dim=128,
            hidden_dim=[64, 64, 64],
            kernel_size=(3, 3),
            num_layers=3,
            batch_first=True,
        )

        # Decoder
        self.decoder = nn.Sequential(
            # upsampling the feature map by a factor of 2
            nn.ConvTranspose2d(64, 64, kernel_size=4, stride=2, padding=1),
            nn.ReLU(inplace=True),
            # further upsampling the featuremap by a factor of 2
            nn.ConvTranspose2d(64, 32, kernel_size=4, stride=2, padding=1),
            nn.ReLU(inplace=True),
            # Optional: additional layers to increase model complexity
            nn.ConvTranspose2d(32, 16, kernel_size=3, stride=1, padding=1),
            nn.ReLU(inplace=True),
            # Final layer, restoring the original number of channels
            nn.ConvTranspose2d(16, 1, kernel_size=3, stride=1, padding=1),
            nn.Tanh(),
        )

    def forward(self, input_imgs):
        batch_size, seq_len, c, h, w = input_imgs.size()
        c_in = input_imgs.view(batch_size * seq_len, c, h, w)
        c_out = self.encoder(c_in)

        # Adjust the shape for ConvLSTM
        c_out = c_out.view(
            batch_size, seq_len, -1, h // 4, w // 4
        )  # Adjusted for the MaxPool layers

        # ConvLSTM processing
        conv_lstm_out, _ = self.conv_lstm(c_out)
        conv_lstm_out = conv_lstm_out[0][
            :, -1, :, :, :
        ]  # Use the output of the last time step

        # Generate the predicted image
        output_image = self.decoder(conv_lstm_out)
        return output_image
