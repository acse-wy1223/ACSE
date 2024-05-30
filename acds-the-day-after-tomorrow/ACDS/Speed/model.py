import torch
import torch.nn as nn
import cv2 # noqa


class SimpleCNN(nn.Module):
    def __init__(self, output_size, image_size):
        super(SimpleCNN, self).__init__()
        self.conv1 = nn.Conv2d(1, 16, kernel_size=5, padding=2)
        self.bn1 = nn.BatchNorm2d(16)
        self.relu = nn.ReLU()
        self.pool = nn.MaxPool2d(2)
        self.conv2 = nn.Conv2d(16, 32, kernel_size=5, padding=2)
        self.bn2 = nn.BatchNorm2d(32)

        reduced_size = image_size // 4
        self.fc = nn.Linear(32 * reduced_size * reduced_size, output_size)

    def forward(self, x):
        x = self.pool(self.relu(self.bn1(self.conv1(x))))
        x = self.pool(self.relu(self.bn2(self.conv2(x))))
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x


class CNNLSTM_V3(nn.Module):
    def __init__(
        self, cnn_output_size, hidden_size, num_layers, num_features=0, image_size=224 # noqa
    ):
        super().__init__()
        self.num_features = num_features
        self.cnn = SimpleCNN(cnn_output_size, image_size)
        self.lstm_additional = nn.LSTM(
            input_size=self.num_features,
            hidden_size=hidden_size,
            num_layers=num_layers,
            batch_first=True,
        )
        self.fc = nn.Sequential(
            nn.Linear(
                cnn_output_size + hidden_size, (cnn_output_size + hidden_size) // 2 # noqa
            ),
            nn.ReLU(),
            nn.Linear((cnn_output_size + hidden_size) // 2, 1),
        )

    def forward(self, image_sequence, additional_features_sequence):
        batch_size, seq_len, C, H, W = image_sequence.size()
        c_in = image_sequence.view(batch_size * seq_len, C, H, W)
        c_out = self.cnn(c_in)
        cnn_features = c_out.view(batch_size, seq_len, -1)
        lstm_out, _ = self.lstm_additional(additional_features_sequence)
        last_time_step = lstm_out[:, -1, :]
        combined_features = torch.cat(
            (cnn_features, last_time_step.unsqueeze(1).repeat(1, seq_len, 1)), 2 # noqa
        )
        return self.fc(combined_features[:, -1, :]).squeeze(1)

    def predict(self, image_sequence, additional_features_sequence):
        return self.forward(image_sequence, additional_features_sequence)
