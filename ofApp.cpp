#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);

	this->hexagon_height = 14;
	this->hexagon_width = 6;

	for (float x = this->hexagon_width * 9; x <= ofGetWidth() - this->hexagon_width * 9; x += this->hexagon_height * 1.5) {

		for (float y = this->hexagon_height * 3; y <= ofGetHeight() - this->hexagon_height * 3; y += this->hexagon_height * 3) {

			this->location_list.push_back(glm::vec2(x, y));
			this->life_list.push_back(0);
			this->noise_param_list.push_back(ofRandom(1000));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	auto noise_location = glm::vec2(ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.008), 0, 1, -60, ofGetWidth() + 60), ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.008), 0, 1, -60, ofGetHeight() + 60));

	for (int i = 0; i < this->location_list.size(); i++) {

		if (glm::distance(noise_location, this->location_list[i]) < 80) {

			this->life_list[i] = this->life_list[i] < 100 ? this->life_list[i] + 4 : 100;
		}
		else {

			this->life_list[i] = this->life_list[i] > 0 ? this->life_list[i] - 0.5 : 0;
		}

		if (this->life_list[i] > 0) {

			this->noise_param_list[i] += ofMap(this->life_list[i], 0, 100, 0.005, 0.1);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofColor fill_color, line_color;
	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->life_list[i] > 0) {

			fill_color = ofColor(39);
			line_color = ofColor(239);
		}
		else {

			fill_color = ofColor(239);
			line_color = ofColor(39);
		}

		int number_index = ofMap(ofNoise(this->noise_param_list[i]), 0, 1, 0, 10);
		this->draw_digital(this->location_list[i], number_index, fill_color, line_color);
	}
}

//--------------------------------------------------------------
void ofApp::draw_digital(glm::vec2 location, int number_index, ofColor fill_color, ofColor line_color) {

	vector<pair<glm::vec2, float>> part_list = {
		make_pair<glm::vec2, float>(location + glm::vec2(0, -this->hexagon_height), 90),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * -0.5, this->hexagon_height * 0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * 0.5, this->hexagon_height * 0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(0, 0), 90),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * -0.5, this->hexagon_height * -0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(this->hexagon_height * 0.5, this->hexagon_height * -0.5), 0),
		make_pair<glm::vec2, float>(location + glm::vec2(0, this->hexagon_height), 90)
	};

	vector<vector<int>> index_list = {
		{ 0, 1, 2, 4, 5, 6 },
		{ 2, 5 },
		{ 0, 1, 3, 5, 6 },
		{ 0, 2, 3, 5, 6 },
		{ 2, 3, 4, 5 },
		{ 0, 2, 3, 4, 6 },
		{ 0, 1, 2, 3, 4, 6 },
		{ 0, 2, 5 },
		{ 0, 1, 2, 3, 4, 5, 6 },
		{ 0, 2, 3, 4, 5, 6 },
	};

	for (auto& index : index_list[number_index]) {

		this->draw_hexagon(part_list[index].first, part_list[index].second, fill_color, line_color);
	}
}

//--------------------------------------------------------------
void ofApp::draw_hexagon(glm::vec2 location, float deg, ofColor fill_color, ofColor line_color) {

	ofPushMatrix();
	ofTranslate(location);
	ofRotate(deg);

	vector<glm::vec2> vertices;
	vertices.push_back(glm::vec2(this->hexagon_width * -0.4, this->hexagon_height * -0.4));
	vertices.push_back(glm::vec2(this->hexagon_width * -0.4, this->hexagon_height * 0.4));
	vertices.push_back(glm::vec2(0, this->hexagon_height * 0.5));
	vertices.push_back(glm::vec2(this->hexagon_width * 0.4, this->hexagon_height * 0.4));
	vertices.push_back(glm::vec2(this->hexagon_width * 0.4, this->hexagon_height * -0.4));
	vertices.push_back(glm::vec2(0, this->hexagon_height * -0.5));

	ofFill();
	ofSetColor(fill_color);

	ofBeginShape();
	ofVertices(vertices);
	ofEndShape(true);

	ofNoFill();
	ofSetColor(line_color);

	ofBeginShape();
	ofVertices(vertices);
	ofEndShape(true);

	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}