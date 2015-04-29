/**
 * Copyright (C) 2007-2010 SlimDX Group
 *
 * Permission is hereby granted, free  of charge, to any person obtaining a copy of this software  and
 * associated  documentation  files (the  "Software"), to deal  in the Software  without  restriction,
 * including  without  limitation  the  rights  to use,  copy,  modify,  merge,  publish,  distribute,
 * sublicense, and/or sell  copies of the  Software,  and to permit  persons to whom  the Software  is
 * furnished to do so, subject to the following conditions:
 *
 * The  above  copyright  notice  and this  permission  notice shall  be included  in  all  copies  or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF  ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A   PARTICULAR  PURPOSE  AND
 * NONINFRINGEMENT.  IN  NO  EVENT SHALL THE  AUTHORS  OR COPYRIGHT HOLDERS  BE LIABLE FOR  ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  OUT
 * OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Matrix.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace GTA
{
	namespace Math
	{
		using namespace System;
		using namespace System::Globalization;

		Quaternion::Quaternion(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}
		Quaternion::Quaternion(Vector3 value, float w)
		{
			X = value.X;
			Y = value.Y;
			Z = value.Z;
			W = w;
		}

		Vector3 Quaternion::Axis::get()
		{
			Vector3 axis;
			float length = Length();

			if (length != 0.0f)
			{
				axis.X = X / length;
				axis.Y = Y / length;
				axis.Z = Z / length;
			}
			else
			{
				axis.X = 1.0f;
				axis.Y = 0.0f;
				axis.Z = 0.0f;
			}

			return axis;
		}
		float Quaternion::Angle::get()
		{
			return (System::Math::Abs(W) <= 1.0f) ? 2.0f * static_cast<float>(System::Math::Acos(W)) : 0.0f;
		}

		float Quaternion::Length()
		{
			return static_cast<float>(System::Math::Sqrt((X * X) + (Y * Y) + (Z * Z) + (W * W)));
		}
		void Quaternion::Normalize()
		{
			float length = 1.0f / Length();
			X *= length;
			Y *= length;
			Z *= length;
			W *= length;
		}
		void Quaternion::Conjugate()
		{
			X = -X;
			Y = -Y;
			Z = -Z;
		}
		void Quaternion::Invert()
		{
			float lengthSq = 1.0f / ((X * X) + (Y * Y) + (Z * Z) + (W * W));
			X = -X * lengthSq;
			Y = -Y * lengthSq;
			Z = -Z * lengthSq;
			W = W * lengthSq;
		}

		Quaternion Quaternion::Add(Quaternion left, Quaternion right)
		{
			Quaternion result;
			result.X = left.X + right.X;
			result.Y = left.Y + right.Y;
			result.Z = left.Z + right.Z;
			result.W = left.W + right.W;
			return result;
		}
		Quaternion Quaternion::Divide(Quaternion left, Quaternion right)
		{
			Quaternion result;
			result.X = left.X / right.X;
			result.Y = left.Y / right.Y;
			result.Z = left.Z / right.Z;
			result.W = left.W / right.W;
			return result;
		}
		float Quaternion::Dot(Quaternion left, Quaternion right)
		{
			return (left.X * right.X) + (left.Y * right.Y) + (left.Z * right.Z) + (left.W * right.W);
		}
		Quaternion Quaternion::Invert(Quaternion quaternion)
		{
			Quaternion result;
			float lengthSq = 1.0f / ((quaternion.X * quaternion.X) + (quaternion.Y * quaternion.Y) + (quaternion.Z * quaternion.Z) + (quaternion.W * quaternion.W));

			result.X = -quaternion.X * lengthSq;
			result.Y = -quaternion.Y * lengthSq;
			result.Z = -quaternion.Z * lengthSq;
			result.W = quaternion.W * lengthSq;

			return result;
		}
		Quaternion Quaternion::Lerp(Quaternion left, Quaternion right, float amount)
		{
			Quaternion result;
			float inverse = 1.0f - amount;
			float dot = (left.X * right.X) + (left.Y * right.Y) + (left.Z * right.Z) + (left.W * right.W);

			if (dot >= 0.0f)
			{
				result.X = (inverse * left.X) + (amount * right.X);
				result.Y = (inverse * left.Y) + (amount * right.Y);
				result.Z = (inverse * left.Z) + (amount * right.Z);
				result.W = (inverse * left.W) + (amount * right.W);
			}
			else
			{
				result.X = (inverse * left.X) - (amount * right.X);
				result.Y = (inverse * left.Y) - (amount * right.Y);
				result.Z = (inverse * left.Z) - (amount * right.Z);
				result.W = (inverse * left.W) - (amount * right.W);
			}

			float invLength = 1.0f / result.Length();

			result.X *= invLength;
			result.Y *= invLength;
			result.Z *= invLength;
			result.W *= invLength;

			return result;
		}
		Quaternion Quaternion::Multiply(Quaternion left, Quaternion right)
		{
			Quaternion quaternion;
			float lx = left.X;
			float ly = left.Y;
			float lz = left.Z;
			float lw = left.W;
			float rx = right.X;
			float ry = right.Y;
			float rz = right.Z;
			float rw = right.W;

			quaternion.X = (lx * rw + rx * lw) + (ly * rz) - (lz * ry);
			quaternion.Y = (ly * rw + ry * lw) + (lz * rx) - (lx * rz);
			quaternion.Z = (lz * rw + rz * lw) + (lx * ry) - (ly * rx);
			quaternion.W = (lw * rw) - (lx * rx + ly * ry + lz * rz);

			return quaternion;
		}
		Quaternion Quaternion::Multiply(Quaternion quaternion, float scale)
		{
			Quaternion result;
			result.X = quaternion.X * scale;
			result.Y = quaternion.Y * scale;
			result.Z = quaternion.Z * scale;
			result.W = quaternion.W * scale;
			return result;
		}
		Quaternion Quaternion::Negate(Quaternion quat)
		{
			Quaternion result;
			result.X = -quat.X;
			result.Y = -quat.Y;
			result.Z = -quat.Z;
			result.W = -quat.W;
			return result;
		}
		Quaternion Quaternion::Normalize(Quaternion quat)
		{
			quat.Normalize();
			return quat;
		}
		Quaternion Quaternion::RotationAxis(Vector3 axis, float angle)
		{
			Quaternion result;

			axis = Vector3::Normalize(axis);

			float half = angle * 0.5f;
			float sin = static_cast<float>(System::Math::Sin(static_cast<double>(half)));
			float cos = static_cast<float>(System::Math::Cos(static_cast<double>(half)));

			result.X = axis.X * sin;
			result.Y = axis.Y * sin;
			result.Z = axis.Z * sin;
			result.W = cos;

			return result;
		}
		Quaternion Quaternion::RotationMatrix(Matrix matrix)
		{
			Quaternion result;
			float scale = matrix.M11 + matrix.M22 + matrix.M33;

			if (scale > 0.0f)
			{
				float sqrt = static_cast<float>(System::Math::Sqrt(static_cast<double>(scale + 1.0f)));

				result.W = sqrt * 0.5f;
				sqrt = 0.5f / sqrt;

				result.X = (matrix.M23 - matrix.M32) * sqrt;
				result.Y = (matrix.M31 - matrix.M13) * sqrt;
				result.Z = (matrix.M12 - matrix.M21) * sqrt;

				return result;
			}

			if ((matrix.M11 >= matrix.M22) && (matrix.M11 >= matrix.M33))
			{
				float sqrt = static_cast<float>(System::Math::Sqrt(static_cast<double>(1.0f + matrix.M11 - matrix.M22 - matrix.M33)));
				float half = 0.5f / sqrt;

				result.X = 0.5f * sqrt;
				result.Y = (matrix.M12 + matrix.M21) * half;
				result.Z = (matrix.M13 + matrix.M31) * half;
				result.W = (matrix.M23 - matrix.M32) * half;

				return result;
			}

			if (matrix.M22 > matrix.M33)
			{
				float sqrt = static_cast<float>(System::Math::Sqrt(static_cast<double>(1.0f + matrix.M22 - matrix.M11 - matrix.M33)));
				float half = 0.5f / sqrt;

				result.X = (matrix.M21 + matrix.M12) * half;
				result.Y = 0.5f * sqrt;
				result.Z = (matrix.M32 + matrix.M23) * half;
				result.W = (matrix.M31 - matrix.M13) * half;

				return result;
			}

			float sqrt = static_cast<float>(System::Math::Sqrt(static_cast<double>(1.0f + matrix.M33 - matrix.M11 - matrix.M22)));
			float half = 0.5f / sqrt;

			result.X = (matrix.M31 + matrix.M13) * half;
			result.Y = (matrix.M32 + matrix.M23) * half;
			result.Z = 0.5f * sqrt;
			result.W = (matrix.M12 - matrix.M21) * half;

			return result;
		}
		Quaternion Quaternion::RotationYawPitchRoll(float yaw, float pitch, float roll)
		{
			Quaternion result;

			float halfRoll = roll * 0.5f;
			float sinRoll = static_cast<float>(System::Math::Sin(static_cast<double>(halfRoll)));
			float cosRoll = static_cast<float>(System::Math::Cos(static_cast<double>(halfRoll)));
			float halfPitch = pitch * 0.5f;
			float sinPitch = static_cast<float>(System::Math::Sin(static_cast<double>(halfPitch)));
			float cosPitch = static_cast<float>(System::Math::Cos(static_cast<double>(halfPitch)));
			float halfYaw = yaw * 0.5f;
			float sinYaw = static_cast<float>(System::Math::Sin(static_cast<double>(halfYaw)));
			float cosYaw = static_cast<float>(System::Math::Cos(static_cast<double>(halfYaw)));

			result.X = (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll);
			result.Y = (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll);
			result.Z = (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll);
			result.W = (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll);

			return result;
		}
		Quaternion Quaternion::Subtract(Quaternion left, Quaternion right)
		{
			Quaternion result;
			result.X = left.X - right.X;
			result.Y = left.Y - right.Y;
			result.Z = left.Z - right.Z;
			result.W = left.W - right.W;
			return result;
		}

		Quaternion Quaternion::operator * (Quaternion left, Quaternion right)
		{
			Quaternion quaternion;
			float lx = left.X;
			float ly = left.Y;
			float lz = left.Z;
			float lw = left.W;
			float rx = right.X;
			float ry = right.Y;
			float rz = right.Z;
			float rw = right.W;

			quaternion.X = (lx * rw + rx * lw) + (ly * rz) - (lz * ry);
			quaternion.Y = (ly * rw + ry * lw) + (lz * rx) - (lx * rz);
			quaternion.Z = (lz * rw + rz * lw) + (lx * ry) - (ly * rx);
			quaternion.W = (lw * rw) - (lx * rx + ly * ry + lz * rz);

			return quaternion;
		}
		Quaternion Quaternion::operator * (Quaternion quaternion, float scale)
		{
			Quaternion result;
			result.X = quaternion.X * scale;
			result.Y = quaternion.Y * scale;
			result.Z = quaternion.Z * scale;
			result.W = quaternion.W * scale;
			return result;
		}
		Quaternion Quaternion::operator * (float scale, Quaternion quaternion)
		{
			Quaternion result;
			result.X = quaternion.X * scale;
			result.Y = quaternion.Y * scale;
			result.Z = quaternion.Z * scale;
			result.W = quaternion.W * scale;
			return result;
		}
		Quaternion Quaternion::operator / (Quaternion lhs, float rhs)
		{
			Quaternion result;
			result.X = lhs.X / rhs;
			result.Y = lhs.Y / rhs;
			result.Z = lhs.Z / rhs;
			result.W = lhs.W / rhs;
			return result;
		}
		Quaternion Quaternion::operator + (Quaternion lhs, Quaternion rhs)
		{
			Quaternion result;
			result.X = lhs.X + rhs.X;
			result.Y = lhs.Y + rhs.Y;
			result.Z = lhs.Z + rhs.Z;
			result.W = lhs.W + rhs.W;
			return result;
		}
		Quaternion Quaternion::operator - (Quaternion lhs, Quaternion rhs)
		{
			Quaternion result;
			result.X = lhs.X - rhs.X;
			result.Y = lhs.Y - rhs.Y;
			result.Z = lhs.Z - rhs.Z;
			result.W = lhs.W - rhs.W;
			return result;
		}
		Quaternion Quaternion::operator - (Quaternion quaternion)
		{
			Quaternion result;
			result.X = -quaternion.X;
			result.Y = -quaternion.Y;
			result.Z = -quaternion.Z;
			result.W = -quaternion.W;
			return result;
		}
		bool Quaternion::operator == (Quaternion left, Quaternion right)
		{
			return Quaternion::Equals(left, right);
		}
		bool Quaternion::operator != (Quaternion left, Quaternion right)
		{
			return !Quaternion::Equals(left, right);
		}

		String ^Quaternion::ToString()
		{
			return String::Format(CultureInfo::InvariantCulture, "X:{0} Y:{1} Z:{2} W:{2}", X.ToString(), Y.ToString(), Z.ToString(), W.ToString());
		}
		int Quaternion::GetHashCode()
		{
			return X.GetHashCode() + Y.GetHashCode() + Z.GetHashCode() + W.GetHashCode();
		}
		bool Quaternion::Equals(Object^ value)
		{
			if (value == nullptr)
				return false;

			if (value->GetType() != GetType())
				return false;

			return Equals(safe_cast<Quaternion>(value));
		}
		bool Quaternion::Equals(Quaternion value)
		{
			return (X == value.X && Y == value.Y && Z == value.Z && W == value.W);
		}
		bool Quaternion::Equals(Quaternion% value1, Quaternion% value2)
		{
			return (value1.X == value2.X && value1.Y == value2.Y && value1.Z == value2.Z && value1.W == value2.W);
		}
	}
}