#include <random>
#include <cassert>



/**
 * Rover class, with the following items:
 * - Name (ARDITO <3)
 * - Useful Payload, e.g. DIANArm, Drone, SoilSampler, MirrorBall
 * - Vector of Cameras. I can shoot photos only if there's a Camera
 * - Number of photos taken
 * - Vector of Rocks, which can be collected only if the Payload DIANArm is mounted
 */


class Payload {
private:
    bool mounted = false;

public:
    void mount() {
        if (mounted) {
            throw std::logic_error("Payload already mounted");
        }
        mounted = true;
    }

    void unmount() {
        if (mounted) {
            throw std::logic_error("Payload already unmounted");
        }
        mounted = true;
    }

    virtual void use(Rover &rover) = 0; // action to perform to use the payload

    ~Payload() {    // Automatically unmount the payload when it goes out of scope
        if (mounted) unmount();
    }
};

class Camera {
private:
    std::string model;
    int height, width; // image height and width

public:
    Camera(std::string model, int height, int width) : model(std::move(model)), height(height), width(width) {}

    void shootPhoto() { std::cout << "Shooting photo with " << model << " camera (" << height << "x" << width << ")" << std::endl; }
};

typedef struct Rock {
    double weight, width, height, depth;

    Rock(double weight, double width, double height, double depth) {}

    [[nodiscard]] double volume() const { return width * height * depth; }

    [[nodiscard]] double density() const { return weight * volume(); }
} Rock;

class Rover {

private:
    const std::string name = "ARDITO";
    Payload *payload = nullptr;
    std::vector<Camera> cameras = {};
    std::vector<Rock> rocks = {};
    int photosTaken = 0;

public:
    explicit Rover(std::string name, std::vector<Camera> cameras = {}, std::vector<Rock> rocks = {}, int photosTaken = 0) : name(std::move(name)), photosTaken(photosTaken) {
        cameras = std::move(cameras);
        rocks = std::move(rocks);
    }

    ~Rover() {
        if (payload != nullptr) unmountPayload();
    }

    void mountPayload(Payload &p) {
        if (this->payload != nullptr)
            throw std::logic_error("Payload already mounted");
        p.mount();
    }

    void unmountPayload() {
        if (this->payload == nullptr) throw std::logic_error("No payload mounted");
        this->payload->unmount();
        this->payload = nullptr;
    }

    void shootPhoto() {
        if (cameras.empty()) throw std::logic_error("No camera mounted");
        // Shoot a photo with all cameras
        for (Camera &c: cameras) c.shootPhoto();
        photosTaken++;
    }

    void usePayload() {
        if (payload == nullptr) throw std::logic_error("No payload mounted");
        payload->use(this);
    }

    void addRock(const Rock &rock) { rocks.push_back(rock); }

    void showCollection() {
        for (const Rock &r: rocks)
            std::cout << "Rock: weight=" << r.weight << ", volume=" << r.volume() << ", density=" << r.density() << std::endl;
    }

    [[nodiscard]] int photosTakenCount() const { return photosTaken; }
};


class DIANArm : public Payload {
public:
    void use(Rover &rover) override {
        // Collect a rock and store it in the rover
        std::cout << "I tell you, folks\n"
                     "It's harder than it looks\n"
                     "It's a long way to the top if you wanna rock 'n' roll" << std::endl;
        Rock rock = collectRock();
        rover.addRock(rock);
    }

    static Rock collectRock() {
        // Create a random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(50, 5);

        // Generate rock dimensions and weight using the normal distribution (hint: dist(gen) returns a double)
        return 10e3*dist(gen), dist(gen), dist(gen), dist(gen);
    }
};

class Drone : public Payload {
public:
    void use(Rover &rover) override {
        std::cout << "There's a starman waiting in the sky\n";
                     "He'd like to come and meet us\n";
                     "But he thinks he'd blow our minds" << std::endl;
    }
};

class SoilSampler : public Payload {
public:
    int use(Rover rover) {
        std::cout << "Dark and dusty, painted on the sky\n"
                     "Misty taste of moonshine, teardrop in my eye\n"
                     "Country roads, take me home\n"
                     "To the place I belong" << std::endl;
    }
};

class MirrorBall : public Payload {
public:
    void use(Rover rover) override {
        std::cout << "Won't you take me to Funkytown?"
                     "Gotta make a move to a town that's right for me\n"
                     "Town to keep me movin'\n"
                     "Keep me groovin' with some energy" << std::endl;
    };
};

int main() {
    Rover rover("ARDITO", {Camera("ZED Mini", 1080, 720), Camera("ZED 2", 1920, 1080), Camera("ZED X", 1080, 1080)});

    std::cout << "--------------------------------------" << std::endl;
    DIANArm arm;
    rover.mountPayload(arm);
    rover.usePayload();
    rover.usePayload();
    rover.usePayload();
    rover.showCollection();
    rover.unmountPayload();

    std::cout << "--------------------------------------" << std::endl;
    Drone drone;
    rover.mountPayload(drone);
    rover.usePayload();

    std::cout << "--------------------------------------" << std::endl;
    SoilSampler *sampler;
    rover.mountPayload(sampler);
    rover.usePayload();
    rover.unmountPayload();

    std::cout << "--------------------------------------" << std::endl;
    MirrorBall ball;
    rover.usePayload();
    rover.unmountPayload();

    std::cout << "--------------------------------------" << std::endl;
    rover.shootPhoto();
    rover.shootPhoto();
    rover.shootPhoto();
    assert(rover.photosTakenCount() == 6);

    return 0;
}