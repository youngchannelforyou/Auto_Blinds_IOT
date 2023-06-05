import pickle
import pandas as pd
from sklearn.pipeline import Pipeline
from sklearn.compose import ColumnTransformer
from sklearn.preprocessing import OneHotEncoder
from sklearn.linear_model import LogisticRegression


class Model:
    model = None

    def train_model(self):
        # 데이터 불러오기
        data = pd.read_csv("blind_data.csv")  # 블라인드 데이터 파일

        # 특성과 타겟 데이터 나누기
        X_train = data.drop("blind_status", axis=1)
        y_train = data["blind_status"]

        # 파이프라인 구성
        categorical_features = ["weather"]
        categorical_transformer = ColumnTransformer(
            transformers=[("encoder", OneHotEncoder(), categorical_features)],
            remainder="passthrough",
        )
        pipeline = Pipeline(
            [
                ("preprocessor", categorical_transformer),
                ("classifier", LogisticRegression()),
            ]
        )

        # 모델 학습
        pipeline.fit(X_train, y_train)

        with open("model.pkl", "wb") as f:
            pickle.dump(pipeline, f)

    def load_model(self):
        with open("model.pkl", "rb") as f:
            loaded_model = pickle.load(f)
        self.model = loaded_model

    def predict(self, new_data):
        prediction = self.model.predict(new_data)  # 커튼을 칠 확률
        return prediction


if __name__ == "__main__":
    my_model = Model()
    my_model.train_model()

    # 모델 불러오기
    my_model.load_model()

    # 예측하기
    time = 1  # 예측할 시간
    illumination = 100  # 조도
    weather = "sun"  # 날씨
    temperature = 1  # 기온

    new_data = pd.DataFrame(
        [[time, illumination, weather, temperature]],
        columns=["time", "illumination", "weather", "temperature"],
    )

    prediction = my_model.predict(new_data)  # 커튼을 칠 확률
    print("커튼:", prediction)
